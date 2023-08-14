////////////////////////////////
//~ rjf: Globals

#if BUILD_CORE
B32 c_initialized = 0;
C_State *c_state = 0;
#endif

////////////////////////////////
//~ rjf: Top-Level API

root_function C_InitReceipt
C_Init(OS_InitReceipt os_init_receipt)
{
 if(IsMainThread() && c_initialized == 0)
 {
  c_initialized = 1;
  Arena *arena = ArenaAlloc(Megabytes(16));
  c_state = PushArray(arena, C_State, 1);
  c_state->arena = arena;
  c_state->tag2data_table_size = 1024;
  c_state->tag2data_table = PushArray(arena, C_Tag2DataCacheSlot, c_state->tag2data_table_size);
  c_state->tag2data_stripes = OS_StripeTableAlloc(arena, c_state->tag2data_table_size/8);
 }
 C_InitReceipt result = {0};
 return result;
}

////////////////////////////////
//~ rjf: Scopes

root_function C_Scope *
C_ScopeOpen(void)
{
 C_Scope *scope = (C_Scope *)1;
 return scope;
}

root_function void
C_ScopeClose(C_Scope *scope)
{
}

////////////////////////////////
//~ rjf: Hashes

root_function C_Hash
C_HashZero(void)
{
 C_Hash hash = {0};
 return hash;
}

root_function C_Hash
C_HashMake(U64 a, U64 b)
{
 C_Hash hash = {a, b};
 return hash;
}

root_function C_Hash
C_HashFromString(String8 string)
{
 C_Hash hash = {0};
 {
  meow_u128 meow_hash = MeowHash(MeowDefaultSeed, string.size, string.str);
  MemoryCopy(&hash, &meow_hash, Min(sizeof(meow_hash), sizeof(hash)));
 }
 return hash;
}

root_function B32
C_HashMatch(C_Hash a, C_Hash b)
{
 return (a.u64[0] == b.u64[0] && a.u64[1] == b.u64[1]);
}

////////////////////////////////
//~ rjf: Cache Interaction

root_function C_Hash
C_SubmitData(Arena **permanent_arena, String8 data)
{
 //- rjf: produce hash from hash
 C_Hash hash = C_HashFromString(data);
 
 //- rjf: hash -> slot/stripe info
 U64 slot_idx = hash.u64[1] % c_state->tag2data_table_size;
 U64 stripe_idx = slot_idx % c_state->tag2data_stripes->count;
 OS_Stripe *stripe = &c_state->tag2data_stripes->stripes[stripe_idx];
 C_Tag2DataCacheSlot *slot = &c_state->tag2data_table[slot_idx];
 
 //- rjf: register in (hash) -> (data) data structure
 B32 is_new = 0;
 OS_MutexBlock(stripe->mutex)
 {
  C_Tag2DataCacheNode *existing_node = 0;
  for(C_Tag2DataCacheNode *n = slot->first; n != 0; n = n->hash_next)
  {
   if(C_HashMatch(n->hash, hash))
   {
    existing_node = n;
    break;
   }
  }
  if(existing_node == 0)
  {
   C_Tag2DataCacheNode *node = PushArray(stripe->arena, C_Tag2DataCacheNode, 1);
   U64 arena_size = data.size;
   arena_size += Kilobytes(64)-1;
   arena_size -= arena_size%Kilobytes(64);
   node->arena = *permanent_arena;
   node->hash = hash;
   node->data = data;
   OS_SetMemoryAccessFlags(data.str, data.size, OS_AccessFlag_Read);
   DLLPushBack_NPZ(slot->first, slot->last, node, hash_next, hash_prev, CheckNull, SetNull);
   is_new = 1;
  }
 }
 
 //- rjf: new -> signal
 if(is_new)
 {
  OS_ConditionVariableSignalAll(stripe->cv);
 }
 
 //- rjf: not new -> release arena
 if(is_new == 0)
 {
  Arena *arena = *permanent_arena;
  ArenaRelease(arena);
 }
 *permanent_arena = 0;
 
 return hash;
}

root_function void
C_SubmitStaticData(String8 data, C_Hash hash)
{
 //- rjf: hash -> slot/stripe info
 U64 slot_idx = hash.u64[1] % c_state->tag2data_table_size;
 U64 stripe_idx = slot_idx % c_state->tag2data_stripes->count;
 OS_Stripe *stripe = &c_state->tag2data_stripes->stripes[stripe_idx];
 C_Tag2DataCacheSlot *slot = &c_state->tag2data_table[slot_idx];
 
 //- rjf: register in (hash) -> (data) data structure
 B32 is_new = 0;
 OS_MutexBlock(stripe->mutex)
 {
  C_Tag2DataCacheNode *existing_node = 0;
  for(C_Tag2DataCacheNode *n = slot->first; n != 0; n = n->hash_next)
  {
   if(C_HashMatch(n->hash, hash))
   {
    existing_node = n;
    break;
   }
  }
  if(existing_node == 0)
  {
   C_Tag2DataCacheNode *node = PushArray(stripe->arena, C_Tag2DataCacheNode, 1);
   node->arena = 0;
   node->hash = hash;
   node->data = data;
   DLLPushBack_NPZ(slot->first, slot->last, node, hash_next, hash_prev, CheckNull, SetNull);
   is_new = 1;
  }
 }
 
 //- rjf: new -> signal
 if(is_new)
 {
  OS_ConditionVariableSignalAll(stripe->cv);
 }
}

root_function String8
C_DataFromHash(C_Scope *scope, C_Hash hash)
{
 String8 data = {0};
 U64 slot_idx = hash.u64[1] % c_state->tag2data_table_size;
 U64 stripe_idx = slot_idx % c_state->tag2data_stripes->count;
 OS_Stripe *stripe = &c_state->tag2data_stripes->stripes[stripe_idx];
 C_Tag2DataCacheSlot *slot = &c_state->tag2data_table[slot_idx];
 OS_MutexBlock(stripe->mutex)
 {
  C_Tag2DataCacheNode *existing_node = 0;
  for(C_Tag2DataCacheNode *n = slot->first; n != 0; n = n->hash_next)
  {
   if(C_HashMatch(n->hash, hash))
   {
    existing_node = n;
    break;
   }
  }
  if(existing_node != 0)
  {
   data = existing_node->data;
   existing_node->last_accessed_time_us = OS_TimeMicroseconds();
  }
 }
 return data;
}
