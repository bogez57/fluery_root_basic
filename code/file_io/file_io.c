////////////////////////////////
//~ rjf: Globals

#if BUILD_ROOT
B32 fs_initialized = 0;
FS_State *fs_state = 0;
#endif

////////////////////////////////
//~ rjf: User Thread APIs

core_function FS_InitReceipt
FS_Init(OS_InitReceipt os_init_receipt, C_InitReceipt c_init_receipt)
{
 if(IsMainThread() && fs_state == 0)
 {
  //- rjf: set up basics
  Arena *arena = ArenaAlloc(Megabytes(256));
  fs_state = PushArray(arena, FS_State, 1);
  fs_state->arena = arena;
  
  //- rjf: set up request ring buffer
  fs_state->req_ring_size = Kilobytes(256);
  fs_state->req_ring_base = PushArrayNoZero(arena, U8, fs_state->req_ring_size);
  fs_state->req_ring_mutex = OS_MutexAlloc();
  fs_state->req_ring_cv = OS_ConditionVariableAlloc();
  
  //- rjf: set up tag table
  fs_state->tag_table_count = 1024;
  fs_state->tag_table_slots = PushArray(fs_state->arena, FS_Slot, fs_state->tag_table_count);
  fs_state->tag_table_stripes = OS_StripeTableAlloc(arena, fs_state->tag_table_count/8);
  
  //- rjf: launch threads
  fs_state->loader_thread_count = OS_LogicalProcessorCount();
  fs_state->loader_threads = PushArray(arena, OS_Handle, fs_state->loader_thread_count);
  for(U64 thread_idx = 0; thread_idx < fs_state->loader_thread_count; thread_idx += 1)
  {
   fs_state->loader_threads[thread_idx] = OS_ThreadStart(0, FS_LoaderThreadEntryPoint);
  }
  fs_state->change_detector_thread = OS_ThreadStart(0, FS_ChangeDetectorThreadEntryPoint);
 }
 FS_InitReceipt result = {0};
 return result;
}

core_function FS_Tag
FS_TagZero(void)
{
 FS_Tag result = {0};
 return result;
}

core_function FS_Tag
FS_TagFromPath(String8 path)
{
 ArenaTemp scratch = GetScratch(0, 0);
 path = OS_NormalizedPathFromStr8(scratch.arena, path);
 
 //- rjf: build tag
 FS_Tag tag = {0};
 {
  meow_u128 hash = MeowHash(MeowDefaultSeed, path.size, path.str);
  MemoryCopy(&tag, &hash, Min(sizeof(tag), sizeof(hash)));
 }
 
 //- rjf: store tag -> path correllation, determine if new
 {
  U64 slot_idx = tag.u64[1] % fs_state->tag_table_count;
  U64 stripe_idx = slot_idx % fs_state->tag_table_stripes->count;
  OS_Stripe *stripe = &fs_state->tag_table_stripes->stripes[stripe_idx];
  FS_Slot *slot = &fs_state->tag_table_slots[slot_idx];
  OS_MutexBlock(stripe->mutex)
  {
   B32 is_new = 1;
   for(FS_Node *n = slot->first; n != 0; n = n->hash_next)
   {
    if(FS_TagMatch(n->tag, tag))
    {
     is_new = 0;
     break;
    }
   }
   if(is_new)
   {
    FS_Node *node = PushArray(stripe->arena, FS_Node, 1);
    node->tag = tag;
    node->path = PushStr8Copy(stripe->arena, path);
    node->hot_version_is_stale = 1;
    QueuePush_NZ(slot->first, slot->last, node, hash_next, CheckNull, SetNull);
   }
  }
 }
 
 ReleaseScratch(scratch);
 return tag;
}

core_function String8
FS_PathFromTag(Arena *arena, FS_Tag tag)
{
 String8 result = {0};
 U64 slot_idx = tag.u64[1] % fs_state->tag_table_count;
 U64 stripe_idx = slot_idx % fs_state->tag_table_stripes->count;
 OS_Stripe *stripe = &fs_state->tag_table_stripes->stripes[stripe_idx];
 FS_Slot *slot = &fs_state->tag_table_slots[slot_idx];
 OS_MutexBlock(stripe->mutex)
 {
  for(FS_Node *n = slot->first; n != 0; n = n->hash_next)
  {
   if(FS_TagMatch(n->tag, tag))
   {
    result = PushStr8Copy(arena, n->path);
    break;
   }
  }
 }
 return result;
}

core_function B32
FS_TagMatch(FS_Tag a, FS_Tag b)
{
 return (a.u64[0] == b.u64[0] && a.u64[1] == b.u64[1]);
}

core_function C_Hash
FS_ContentHashFromTag(FS_Tag tag, U64 endt_microseconds)
{
 C_Hash hash = {0};
 if(!FS_TagMatch(tag, FS_TagZero()))
 {
  U64 slot_idx = tag.u64[1] % fs_state->tag_table_count;
  U64 stripe_idx = slot_idx % fs_state->tag_table_stripes->count;
  OS_Stripe *stripe = &fs_state->tag_table_stripes->stripes[stripe_idx];
  FS_Slot *slot = &fs_state->tag_table_slots[slot_idx];
  OS_MutexBlock(stripe->mutex)
  {
   for(;;)
   {
    for(FS_Node *n = slot->first; n != 0; n = n->hash_next)
    {
     if(FS_TagMatch(n->tag, tag))
     {
      if(n->data_has_been_queried == 0)
      {
       n->data_has_been_queried = 1;
       FS_EnqueueLoadRequest(tag);
      }
      else if(!C_HashMatch(n->hash, C_HashZero()))
      {
       hash = n->hash;
       goto end;
      }
     }
    }
    if(OS_TimeMicroseconds() >= endt_microseconds)
    {
     break;
    }
    OS_ConditionVariableWait(stripe->cv, stripe->mutex, endt_microseconds);
   }
   end:;
  }
 }
 return hash;
}

core_function void
FS_EnqueueLoadRequest(FS_Tag tag)
{
 U64 bytes_needed = sizeof(tag);
 OS_MutexBlock(fs_state->req_ring_mutex) for(;;)
 {
  if(fs_state->req_ring_write_pos - fs_state->req_ring_read_pos < fs_state->req_ring_size - bytes_needed)
  {
   RingWrite(fs_state->req_ring_base, fs_state->req_ring_size, fs_state->req_ring_write_pos, Str8Struct(&tag));
   fs_state->req_ring_write_pos += bytes_needed;
   AtomicIncEval64(&fs_state->loader_thread_request_count);
   break;
  }
  OS_ConditionVariableWait(fs_state->req_ring_cv, fs_state->req_ring_mutex, U64Max);
 }
 OS_ConditionVariableSignalAll(fs_state->req_ring_cv);
}

core_function FS_Tag
FS_DequeueLoadRequest(void)
{
 FS_Tag result = {0};
 OS_MutexBlock(fs_state->req_ring_mutex) for(;;)
 {
  if(fs_state->req_ring_read_pos < fs_state->req_ring_write_pos)
  {
   RingRead(&result, fs_state->req_ring_base, fs_state->req_ring_size, fs_state->req_ring_read_pos, sizeof(result));
   fs_state->req_ring_read_pos += sizeof(result);
   break;
  }
  OS_ConditionVariableWait(fs_state->req_ring_cv, fs_state->req_ring_mutex, U64Max);
 }
 OS_ConditionVariableSignalAll(fs_state->req_ring_cv);
 return result;
}

core_function S64
FS_LoaderThreadWorkingCount(void)
{
 return fs_state->loader_thread_working_count;
}

core_function S64
FS_LoaderThreadRequestCount(void)
{
 return fs_state->loader_thread_request_count;
}

////////////////////////////////
//~ rjf: Worker Thread Implementations

core_function void
FS_LoaderThreadEntryPoint(void *p)
{
 for(;;)
 {
  ArenaTemp scratch = GetScratch(0, 0);
  
  //- rjf: get next request
  FS_Tag tag = FS_DequeueLoadRequest();
  
  //- rjf: get slot/stripe info for this tag
  U64 slot_idx = tag.u64[1] % fs_state->tag_table_count;
  U64 stripe_idx = slot_idx % fs_state->tag_table_stripes->count;
  OS_Stripe *stripe = &fs_state->tag_table_stripes->stripes[stripe_idx];
  FS_Slot *slot = &fs_state->tag_table_slots[slot_idx];
  
  //- rjf: check if we've already loaded this file
  String8 path = {0};
  B32 need_reload = 0;
  OS_MutexBlock(stripe->mutex)
  {
   FS_Node *existing_node = 0;
   for(FS_Node *n = slot->first; n != 0; n = n->hash_next)
   {
    if(FS_TagMatch(n->tag, tag))
    {
     existing_node = n;
     need_reload = n->hot_version_is_stale;
     break;
    }
   }
   if(existing_node != 0)
   {
    path = PushStr8Copy(scratch.arena, existing_node->path);
   }
  }
  
  //- rjf: bump working loader counter
  if(need_reload)
  {
   AtomicIncEval64(&fs_state->loader_thread_working_count);
  }
  
  //- rjf: load file
  OS_ErrorList errors = {0};
  Arena *data_arena = 0;
  String8 data = {0};
  OS_Timestamp last_modified = 0;
  if(need_reload)
  {
   OS_FileAttributes attributes = OS_FileAttributesFromPath(path);
   last_modified = attributes.last_modified;
   OS_Handle file = OS_FileOpen(scratch.arena, OS_AccessFlag_Read, path, &errors);
   OS_FileAttributes atts = OS_AttributesFromFile(file);
   U64 size = atts.size;
   U64 arena_size = size+Kilobytes(64)-1;
   arena_size -= arena_size%Kilobytes(64);
   if(size > 0 && arena_size > 0)
   {
    data_arena = ArenaAlloc(arena_size);
    data = OS_FileRead(data_arena, file, R1U64(0, size), &errors);
   }
   OS_FileClose(file);
  }
  
  //- rjf: submit data blob to content system
  C_Hash hash = {0};
  if(data_arena != 0 && need_reload)
  {
   hash = C_SubmitData(&data_arena, data);
  }
  
  //- rjf: update existing correllation with new hash & modified time if reload
  if(need_reload)
  {
   OS_MutexBlock(stripe->mutex)
   {
    FS_Node *node = 0;
    for(FS_Node *n = slot->first; n != 0; n = n->hash_next)
    {
     if(FS_TagMatch(n->tag, tag))
     {
      node = n;
      break;
     }
    }
    Assert(node != 0);
    node->hash = hash;
    node->last_modified = last_modified;
   }
   OS_ConditionVariableSignalAll(stripe->cv);
  }
  
  //- rjf: dec counters
  if(need_reload)
  {
   AtomicDecEval64(&fs_state->loader_thread_working_count);
  }
  AtomicDecEval64(&fs_state->loader_thread_request_count);
  
  ReleaseScratch(scratch);
 }
}

core_function void
FS_ChangeDetectorThreadEntryPoint(void *p)
{
 for(;;)
 {
  for(U64 slot_idx = 0; slot_idx < fs_state->tag_table_count; slot_idx += 1)
  {
   // rjf: get slot
   FS_Slot *slot = &fs_state->tag_table_slots[slot_idx];
   
   // rjf: get stripe info
   U64 stripe_idx = slot_idx % fs_state->tag_table_stripes->count;
   OS_Stripe *stripe = &fs_state->tag_table_stripes->stripes[stripe_idx];
   
   // rjf: check this slot's files for changes - mark nodes appropriately
   B32 changes = 0;
   OS_MutexBlock(stripe->mutex)
   {
    for(FS_Node *n = slot->first; n != 0; n = n->hash_next)
    {
     if(n->data_has_been_queried)
     {
      OS_FileAttributes attributes = OS_FileAttributesFromPath(n->path);
      OS_Timestamp last_modified = attributes.last_modified;
      if(last_modified != n->last_modified)
      {
       n->hot_version_is_stale = 1;
       changes = 1;
       FS_EnqueueLoadRequest(n->tag);
      }
     }
    }
   }
   
   // rjf: wake up
   if(changes)
   {
    OS_ConditionVariableSignalAll(stripe->cv);
   }
  }
  
  OS_Sleep(100);
 }
}
