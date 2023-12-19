////////////////////////////////
//~ rjf: Globals

#if BUILD_CORE
T_State *t_state = 0;
#endif

////////////////////////////////
//~ rjf: Main API

root_function T_InitReceipt
T_Init(C_InitReceipt c_init_receipt, R_InitReceipt r_init_receipt)
{
 if(IsMainThread() && t_state == 0)
 {
  Arena *arena = ArenaAlloc(Megabytes(8));
  t_state = PushArray(arena, T_State, 1);
  
  //- rjf: initialize main state
  t_state->arena = arena;
  U32 invalid_texture_rgba[] =
  {
   0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff,
   0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff,
   0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff,
   0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff,
   0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff,
   0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff,
   0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff,
   0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff, 0x000000ff, 0xff0000ff,
  };
  t_state->invalid_texture = R_Tex2DAlloc(V2S64(8, 8), R_Tex2DFormat_RGBA8, R_Tex2DKind_Static, &invalid_texture_rgba[0]);
  
  //- rjf: initialize load request queue
  t_state->req_ring_size = Kilobytes(256);
  t_state->req_ring_base = PushArrayNoZero(t_state->arena, U8, t_state->req_ring_size);
  t_state->req_ring_cv = OS_ConditionVariableAlloc();
  t_state->req_ring_mutex = OS_MutexAlloc();
  
  //- rjf: initialize shared data structure for (tag) -> (tex)
  t_state->tex_table_size = 256;
  t_state->tex_table_stripe_count = t_state->tex_table_size/8;
  t_state->tex_table = PushArray(arena, T_Slot, t_state->tex_table_size);
  t_state->tex_table_stripe_mutexes = PushArray(arena, OS_Handle, t_state->tex_table_stripe_count);
  t_state->tex_table_stripe_cvs = PushArray(arena, OS_Handle, t_state->tex_table_stripe_count);
  t_state->tex_table_stripe_arenas = PushArray(arena, Arena *, t_state->tex_table_stripe_count);
  for(U64 stripe_idx = 0; stripe_idx < t_state->tex_table_stripe_count; stripe_idx += 1)
  {
   t_state->tex_table_stripe_mutexes[stripe_idx] = OS_MutexAlloc();
   t_state->tex_table_stripe_cvs[stripe_idx] = OS_ConditionVariableAlloc();
   t_state->tex_table_stripe_arenas[stripe_idx] = ArenaAlloc(Gigabytes(1));
  }
  
  //- rjf: prep atlas data structure
  t_state->atlas_list_mutex = OS_MutexAlloc();
  
  //- rjf: launch gpu upload threads
  t_state->thread_count = 1;
  t_state->threads = PushArray(t_state->arena, OS_Handle, t_state->thread_count);
  for(U64 thread_idx = 0; thread_idx < t_state->thread_count; thread_idx += 1)
  {
   t_state->threads[thread_idx] = OS_ThreadStart((void *)thread_idx, T_ThreadEntryPoint);
  }
 }
 T_InitReceipt receipt = {0};
 return receipt;
}

////////////////////////////////
//~ rjf: Accessors

root_function R_Handle
T_InvalidTexture(void)
{
 return t_state->invalid_texture;
}

////////////////////////////////
//~ rjf: Tag -> Texture Region Mapping

root_function R_Slice2F32
T_Slice2F32FromHash(U128 hash, U64 endt_microseconds)
{
 //- rjf: map hash to slot/stripe info
 U64 slot_idx = hash.u64[1] % t_state->tex_table_size;
 U64 stripe_idx = slot_idx % t_state->tex_table_stripe_count;
 T_Slot *slot = &t_state->tex_table[slot_idx];
 OS_Handle stripe_cv = t_state->tex_table_stripe_cvs[stripe_idx];
 OS_Handle stripe_mutex = t_state->tex_table_stripe_mutexes[stripe_idx];
 
 //- rjf: check slot for this hash
 R_Slice2F32 result = {0};
 {
  Vec2S64 invalid_texture_size = R_SizeFromTex2D(t_state->invalid_texture);
  result.texture = t_state->invalid_texture;
  result.region = R2F32(V2F32(0, 0), Vec2F32FromVec(invalid_texture_size));
 }
 OS_MutexScope(stripe_mutex)
 {
  for(;;)
  {
   T_Node *node = 0;
   for(T_Node *n = slot->first; n != 0; n = n->hash_next)
   {
    if(U128Match(n->hash, hash))
    {
     node = n;
     break;
    }
   }
   if(node != 0)
   {
    switch(node->alloc_kind)
    {
     default:break;
     case T_NodeAllocKind_Atlas:
     {
      T_Atlas *atlas = 0;
      OS_MutexScope(t_state->atlas_list_mutex)
      {
       S32 num = 1;
       for(T_Atlas *a = t_state->first_atlas; a != 0; a = a->next, num += 1)
       {
        if(num == node->alloc_atlas.atlas_num)
        {
         atlas = a;
         break;
        }
       }
      }
      if(atlas != 0 && node->alloc_atlas.alloc_region.p1.x != node->alloc_atlas.alloc_region.p0.x)
      {
       result.texture = atlas->texture;
       result.region.p0 = Vec2F32FromVec(node->alloc_atlas.alloc_region.p0);
       result.region.p1 = Add2F32(result.region.p0, node->dim);
      }
     }break;
     case T_NodeAllocKind_Standalone:
     {
      result.texture = node->alloc_standalone.texture;
      result.region.p0 = V2F32(0, 0);
      result.region.p1 = Add2F32(result.region.p0, node->dim);
     }break;
    }
    break;
   }
   else
   {
    U64 time_us = OS_TimeMicroseconds();
    T_EnqueueLoadRequest(hash);
    if(time_us >= endt_microseconds)
    {
     break;
    }
    OS_ConditionVariableWait(stripe_cv, stripe_mutex, endt_microseconds);
   }
  }
 }
 
 return result;
}

////////////////////////////////
//~ rjf: GPU Upload Request Ring Buffer

root_function B32
T_EnqueueLoadRequest(U128 hash)
{
 B32 result = 0;
 OS_MutexScopeEnter(t_state->req_ring_mutex);
 U64 needed_size = sizeof(U128);
 
 //- rjf: room to push request -> push!
 if(t_state->req_ring_write_pos < t_state->req_ring_read_pos + t_state->req_ring_size - needed_size)
 {
  result = 1;
  
  // rjf: write
  RingWriteStruct(t_state->req_ring_base, t_state->req_ring_size, t_state->req_ring_write_pos, &hash);
  
  // rjf: advance write position
  t_state->req_ring_write_pos += needed_size;
  t_state->req_ring_write_pos += 7;
  t_state->req_ring_write_pos -= t_state->req_ring_write_pos%8;
 }
 
 OS_MutexScopeLeave(t_state->req_ring_mutex);
 OS_ConditionVariableSignalAll(t_state->req_ring_cv);
 return result;
}

root_function U128
T_DequeueLoadRequest(void)
{
 U128 hash = {0};
 OS_MutexScope(t_state->req_ring_mutex) for(;;)
 {
  if(t_state->req_ring_write_pos >= t_state->req_ring_read_pos + sizeof(U128))
  {
   t_state->req_ring_read_pos += RingReadStruct(t_state->req_ring_base, t_state->req_ring_size, t_state->req_ring_read_pos, &hash);
   t_state->req_ring_read_pos += 7;
   t_state->req_ring_read_pos -= t_state->req_ring_read_pos%8;
   break;
  }
  OS_ConditionVariableWait(t_state->req_ring_cv, t_state->req_ring_mutex, U64Max);
 }
 return hash;
}

////////////////////////////////
//~ rjf: GPU Data Upload Thread Implementation

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
T_ThreadEntryPoint(void *p)
{
 SetThreadNameF("[T] #%I64u", (U64)p);
 for(;;)
 {
  C_Scope *scope = C_ScopeOpen();
  
  //- rjf: get next request
  U128 hash = T_DequeueLoadRequest();
  
  //- rjf: map hash to slot/stripe info
  U64 slot_idx = hash.u64[1] % t_state->tex_table_size;
  U64 stripe_idx = slot_idx % t_state->tex_table_stripe_count;
  T_Slot *slot = &t_state->tex_table[slot_idx];
  OS_Handle stripe_mutex = t_state->tex_table_stripe_mutexes[stripe_idx];
  OS_Handle stripe_cv = t_state->tex_table_stripe_cvs[stripe_idx];
  Arena *stripe_arena = t_state->tex_table_stripe_arenas[stripe_idx];
  
  //- rjf: check if loaded
  B32 is_already_loaded = 0;
  OS_MutexScope(stripe_mutex)
  {
   for(T_Node *n = slot->first; n != 0; n = n->hash_next)
   {
    if(U128Match(n->hash, hash))
    {
     is_already_loaded = 1;
     break;
    }
   }
  }
  
  //- rjf: hash -> file data
  String8 file_data = {0};
  if(!is_already_loaded)
  {
   file_data = C_DataFromHash(scope, hash);
  }
  
  //- rjf: file data -> raw pixel data
  Vec2S64 size = V2S64(0, 0);
  String8 raw_data = {0};
  if(!is_already_loaded)
  {
   int width = 0;
   int height = 0;
   int components = 0;
   raw_data.str = stbi_load_from_memory(file_data.str, file_data.size, &width, &height, &components, 4);
   raw_data.size = width*height*4;
   size.x = width;
   size.y = height;
  }
  
  //- rjf: allocate node in cache
  if(!is_already_loaded && raw_data.size != 0 && size.x != 0 && size.y != 0)
  {
   // rjf: get dimensions
   Vec2F32 dim = Vec2F32FromVec(size);
   
   // rjf: pick allocation kind based on dimensions
   T_NodeAllocKind alloc_kind = T_NodeAllocKind_Null;
   {
    F32 dim_ratio = dim.x/dim.y;
    if(dim.x > 1024 || dim.y > 1024 || AbsoluteValueF32(1.f - dim_ratio) > 0.4f)
    {
     alloc_kind = T_NodeAllocKind_Standalone;
    }
    else
    {
     alloc_kind = T_NodeAllocKind_Atlas;
    }
   }
   
   // rjf: pick atlas & allocate from it
   Rng2S64 atlas_alloc_region = {0};
   S32 atlas_num = 1;
   if(alloc_kind == T_NodeAllocKind_Atlas) OS_MutexScope(t_state->atlas_list_mutex)
   {
    for(T_Atlas *a = t_state->first_atlas;; a = a->next, atlas_num += 1)
    {
     // rjf: no atlas? => generate new
     if(a == 0)
     {
      Arena *atlas_arena = ArenaAlloc(Megabytes(64));
      a = PushArray(atlas_arena, T_Atlas, 1);
      a->mutex = OS_MutexAlloc();
      a->arena = atlas_arena;
      a->atlas_dim = V2S64(1024, 1024);
      a->atlas = AtlasMake(a->arena, a->atlas_dim);
      a->texture = R_Tex2DAlloc(a->atlas_dim, R_Tex2DFormat_RGBA8, R_Tex2DKind_Dynamic, 0);
      DLLPushBack_NPZ(t_state->first_atlas, t_state->last_atlas, a, next, prev, CheckNull, SetNull);
     }
     
     // rjf: try to allocate from this atlas
     B32 success = 0;
     OS_MutexScope(a->mutex)
     {
      atlas_alloc_region = AtlasRegionAlloc(a->arena, a->atlas, size);
      
      // rjf: determine success
      success = atlas_alloc_region.p1.x != atlas_alloc_region.p0.x;
      
      // rjf: success => upload contents to atlas
      if(success)
      {
       R_Tex2DFillRegion(a->texture, R2S64(atlas_alloc_region.p0, Add2S64(atlas_alloc_region.p0, size)), raw_data.str);
      }
     }
     
     // rjf: success => break
     if(success)
     {
      break;
     }
    }
   }
   
   // rjf: store in shared cache data structure
   OS_MutexScope(stripe_mutex)
   {
    T_Node *node = PushArray(stripe_arena, T_Node, 1);
    DLLPushBack_NPZ(slot->first, slot->last, node, hash_next, hash_prev, CheckNull, SetNull);
    node->hash = hash;
    node->dim = dim;
    node->flags = T_NodeFlag_Hot;
    node->alloc_kind = alloc_kind;
    switch(node->alloc_kind)
    {
     case T_NodeAllocKind_Atlas:
     {
      node->alloc_atlas.alloc_region = atlas_alloc_region;
      node->alloc_atlas.atlas_num = atlas_num;
     }break;
     case T_NodeAllocKind_Standalone:
     {
      node->alloc_standalone.texture = R_Tex2DAlloc(size, R_Tex2DFormat_RGBA8, R_Tex2DKind_Static, raw_data.str);
     }break;
    }
   }
   
   // rjf: announce
   OS_ConditionVariableSignalAll(stripe_cv);
  }
  
  //- rjf: release raw data
  if(!is_already_loaded)
  {
   stbi_image_free(raw_data.str);
  }
  
  C_ScopeClose(scope);
 }
}
