////////////////////////////////
//~ rjf: Globals

#if BUILD_CORE
BC_State *bc_state = 0;
BC_Bitmap bc_nil_bitmap = {0};
#endif

////////////////////////////////
//~ rjf: Top-Level API

root_function BC_InitReceipt
BC_Init(OS_InitReceipt os_init, C_InitReceipt c_init)
{
 if(IsMainThread() && bc_state == 0)
 {
  Arena *arena = ArenaAlloc(Gigabytes(1));
  bc_state = PushArray(arena, BC_State, 1);
  bc_state->arena = arena;
  bc_state->slots_count = 256;
  bc_state->slots = PushArray(arena, BC_Slot, bc_state->slots_count);
  bc_state->stripes = OS_StripeTableAlloc(arena, bc_state->slots_count/32);
  bc_state->u2p_ring_size = Kilobytes(64);
  bc_state->u2p_ring_base = PushArrayNoZero(arena, U8, bc_state->u2p_ring_size);
  bc_state->u2p_ring_cv = OS_ConditionVariableAlloc();
  bc_state->u2p_ring_mutex = OS_MutexAlloc();
  bc_state->p_thread_count = Max(1, OS_LogicalProcessorCount()/2);
  bc_state->p_threads = PushArray(arena, OS_Handle, bc_state->p_thread_count);
  for(U64 idx = 0; idx < bc_state->p_thread_count; idx += 1)
  {
   bc_state->p_threads[idx] = OS_ThreadStart((void *)idx, BC_ParseThreadEntryPoint);
  }
 }
 BC_InitReceipt receipt = {0};
 return receipt;
}

////////////////////////////////
//~ rjf: Scopes

root_function BC_Scope *
BC_ScopeOpen(void)
{
 BC_Scope *scope = (BC_Scope *)1;
 return scope;
}

root_function void
BC_ScopeClose(BC_Scope *scope)
{
}

////////////////////////////////
//~ rjf: Cache Interaction

root_function BC_Bitmap *
BC_BitmapFromHash(BC_Scope *scope, U128 hash, U64 end_time_microseconds)
{
 BC_Bitmap *bitmap = &bc_nil_bitmap;
 
 //- rjf: hash -> slot/stripe
 U64 slot_idx = hash.u64[1]%bc_state->slots_count;
 BC_Slot *slot = &bc_state->slots[slot_idx];
 U64 stripe_idx = slot_idx%bc_state->stripes->count;
 OS_Stripe *stripe = bc_state->stripes->stripes + stripe_idx;
 
 //- rjf: get buffer
 OS_MutexScope(stripe->mutex)
 {
  // rjf: get existing node
  BC_Node *existing_node = 0;
  for(BC_Node *node = slot->first; node != 0; node = node->next)
  {
   if(U128Match(node->hash, hash))
   {
    existing_node = node;
    break;
   }
  }
  
  // rjf: nonzero node, good bitmap -> grab bitmap
  if(existing_node != 0 && existing_node->bitmap != 0)
  {
   bitmap = existing_node->bitmap;
  }
  
  // rjf: nonzero node, no bitmap -> re-enqueue every so often
  else if(existing_node != 0 && existing_node->last_t_queried_us+200 < OS_TimeMicroseconds())
  {
   existing_node->last_t_queried_us = OS_TimeMicroseconds();
   BC_EnqueueParseRequest(hash);
  }
  
  // rjf: no node -> build node & queue request
  else if(existing_node == 0)
  {
   existing_node = PushArray(stripe->arena, BC_Node, 1);
   DLLPushBack(slot->first, slot->last, existing_node);
   existing_node->hash = hash;
   existing_node->last_t_queried_us = OS_TimeMicroseconds();
   BC_EnqueueParseRequest(hash);
  }
 }
 
 return bitmap;
}

////////////////////////////////
//~ rjf: User -> Parsing Thread Ring Buffer

root_function void
BC_EnqueueParseRequest(U128 hash)
{
 OS_MutexScope(bc_state->u2p_ring_mutex) for(;;)
 {
  if((bc_state->u2p_ring_write_pos - bc_state->u2p_ring_read_pos) <= bc_state->u2p_ring_size - sizeof(U128))
  {
   U64 write_pos = bc_state->u2p_ring_write_pos;
   write_pos += RingWriteStruct(bc_state->u2p_ring_base, bc_state->u2p_ring_size, write_pos, &hash);
   bc_state->u2p_ring_write_pos = write_pos;
   break;
  }
  OS_ConditionVariableWait(bc_state->u2p_ring_cv, bc_state->u2p_ring_mutex, U64Max);
 }
 OS_ConditionVariableSignalAll(bc_state->u2p_ring_cv);
}

root_function U128
BC_DequeueParseRequest(void)
{
 U128 hash = {0};
 OS_MutexScope(bc_state->u2p_ring_mutex) for(;;)
 {
  if(bc_state->u2p_ring_write_pos >= bc_state->u2p_ring_read_pos + sizeof(U128))
  {
   U64 read_pos = bc_state->u2p_ring_read_pos;
   read_pos += RingReadStruct(bc_state->u2p_ring_base, bc_state->u2p_ring_size, read_pos, &hash);
   bc_state->u2p_ring_read_pos = read_pos;
   break;
  }
  OS_ConditionVariableWait(bc_state->u2p_ring_cv, bc_state->u2p_ring_mutex, U64Max);
 }
 OS_ConditionVariableSignalAll(bc_state->u2p_ring_cv);
 return hash;
}

////////////////////////////////
//~ rjf: Parsing Threads

#if !defined(STB_IMAGE_INCLUDED)
#define STB_IMAGE_INCLUDED
#define STBI_ONLY_PNG
#define STBI_ONLY_JPEG
#define STBI_ONLY_BMP
#define STBI_ONLY_PSD
#define STBI_ONLY_TGA
#define STB_IMAGE_IMPLEMENTATION
#include "third_party/stb_image.h"
#endif

root_function void
BC_ParseThreadEntryPoint(void *p)
{
 SetThreadNameF("[BC] Parser #%I64u", (U64)p);
 for(;;)
 {
  C_Scope *scope = C_ScopeOpen();
  
  //- rjf: dequeue request & get hash data
  U128 hash = BC_DequeueParseRequest();
  String8 data = C_DataFromHash(scope, hash);
  U64 slot_idx = hash.u64[1]%bc_state->slots_count;
  BC_Slot *slot = &bc_state->slots[slot_idx];
  U64 stripe_idx = slot_idx%bc_state->stripes->count;
  OS_Stripe *stripe = bc_state->stripes->stripes + stripe_idx;
  
  //- rjf: check if loaded
  B32 is_already_loaded = 0;
  OS_MutexScope(stripe->mutex)
  {
   for(BC_Node *n = slot->first; n != 0; n = n->next)
   {
    if(U128Match(n->hash, hash) && n->bitmap != 0)
    {
     is_already_loaded = 1;
     break;
    }
   }
  }
  
  //- rjf: file data -> raw pixel data
  Vec2S64 size = V2S64(0, 0);
  String8 raw_data = {0};
  if(!is_already_loaded)
  {
   int width = 0;
   int height = 0;
   int components = 0;
   raw_data.str = stbi_load_from_memory(data.str, data.size, &width, &height, &components, 4);
   raw_data.size = width*height*4;
   size.x = width;
   size.y = height;
  }
  
  //- rjf: fill node in cache
  if(!is_already_loaded)
  {
   OS_MutexScope(stripe->mutex)
   {
    // rjf: get existing node
    BC_Node *node = 0;
    for(BC_Node *n = slot->first; n != 0; n = n->next)
    {
     if(U128Match(n->hash, hash))
     {
      node = n;
      break;
     }
    }
    
    // rjf: fill node
    if(node != 0)
    {
     U64 needed_arena_size = raw_data.size + sizeof(BC_Bitmap);
     needed_arena_size += Kilobytes(64)-1;
     needed_arena_size -= needed_arena_size%Kilobytes(64);
     node->arena = ArenaAlloc(needed_arena_size);
     node->bitmap = PushArray(node->arena, BC_Bitmap, 1);
     node->bitmap->pixels = PushArrayNoZero(node->arena, U8, raw_data.size);
     MemoryCopy(node->bitmap->pixels, raw_data.str, raw_data.size);
     node->bitmap->dim = size;
     node->bitmap->bytes_per_pixel = 4;
    }
   }
  }
  
  C_ScopeClose(scope);
 }
}
