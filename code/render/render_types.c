////////////////////////////////
//~ rjf: Handle Type Functions

root_function R_Handle
R_HandleZero(void)
{
 R_Handle result = {0};
 return result;
}

root_function B32
R_HandleMatch(R_Handle a, R_Handle b)
{
 return (a.u64[0] == b.u64[0] &&
         a.u64[1] == b.u64[1] &&
         a.u64[2] == b.u64[2] &&
         a.u64[3] == b.u64[3]);
}

root_function B32
R_HandleIsZero(R_Handle handle)
{
 return R_HandleMatch(handle, R_HandleZero());
}

////////////////////////////////
//~ rjf: Texture Type Functions

root_function U64
R_BytesPerPixelFromTex2DFormat(R_Tex2DFormat fmt)
{
 U64 result = 0;
 switch(fmt)
 {
  default:
  case R_Tex2DFormat_R8:   {result = 1;}break;
  case R_Tex2DFormat_RGBA8:{result = 4;}break;
 }
 return result;
}

////////////////////////////////
//~ rjf: Pass Building Helper Functions

//- rjf: pass list building

root_function R_Pass *
R_PassListPush(Arena *arena, R_PassList *list, R_PassKind kind)
{
 R_PassNode *node = PushArray(arena, R_PassNode, 1);
 QueuePush(list->first, list->last, node);
 list->count += 1;
 R_Pass *pass = &node->v;
 pass->kind = kind;
 switch(kind)
 {
  default:{}break;
  case R_PassKind_UI:{pass->params_ui = PushArray(arena, R_PassParams_UI, 1);}break;
  case R_PassKind_G0:{pass->params_g0 = PushArray(arena, R_PassParams_G0, 1);}break;
 }
 return pass;
}

root_function void
R_PassListConcatInPlace(R_PassList *dst, R_PassList *src)
{
 if(dst->last == 0)
 {
  MemoryCopyStruct(dst, src);
 }
 else if(src->first != 0)
 {
  dst->last->next = src->first;
  dst->last = src->last;
  dst->count += src->count;
 }
 MemoryZeroStruct(src);
}

//- rjf: batch list pushing

root_function void *
R_BatchListPush(Arena *arena, R_BatchList *list, U64 cap, U64 instance_size)
{
 void *result = 0;
 
 // rjf: obtain batch. three possibilities:
 // (a) no batch -> create batch
 // (b) batch full & can grow -> try to grow batch if it'd be contiguous on the same arena
 // (c) batch full & cannot grow -> create new batch
 R_Batch *batch = list->last;
 if(batch == 0 || batch->inst_count >= batch->inst_cap)
 {
  void *try_new_instance = ArenaPushNoZero(arena, instance_size);
  void *end_of_batch_block = batch ? ((U8 *)batch->v + instance_size*batch->inst_cap) : 0;
  if(batch != 0 && batch->inst_count == batch->inst_cap && try_new_instance == end_of_batch_block)
  {
   batch->inst_cap += 1;
   batch->byte_cap += instance_size;
  }
  else
  {
   ArenaPop(arena, instance_size);
   batch = PushArray(arena, R_Batch, 1);
   batch->v = PushArrayNoZero(arena, U8, instance_size*cap);
   batch->inst_cap = cap;
   batch->byte_cap = instance_size*cap;
   QueuePush(list->first, list->last, batch);
   list->batch_count += 1;
  }
 }
 
 // rjf: grab ptr to new instance & increment
 result = &batch->v[batch->inst_count*instance_size];
 batch->inst_count += 1;
 batch->byte_count += instance_size;
 list->inst_count += 1;
 list->byte_count += instance_size;
 
 return result;
}

root_function void
R_BatchListConcatInPlace(R_BatchList *list, R_BatchList *to_push)
{
 if(list->last && to_push->first)
 {
  list->last->next = to_push->first;
  list->last = to_push->last;
  list->batch_count += to_push->batch_count;
  list->inst_count += to_push->inst_count;
  list->byte_count += to_push->byte_count;
 }
 else
 {
  MemoryCopyStruct(list, to_push);
 }
 MemoryZeroStruct(to_push);
}

root_function void
R_BatchListConcatDeepCopy(Arena *arena, R_BatchList *list, R_BatchList *to_push)
{
 for(R_Batch *src_batch = to_push->first;
     src_batch != 0;
     src_batch = src_batch->next)
 {
  R_Batch *dst_batch = PushArray(arena, R_Batch, 1);
  dst_batch->v = PushArrayNoZero(arena, U8, src_batch->byte_count);
  dst_batch->byte_count = src_batch->byte_count;
  dst_batch->byte_cap = src_batch->byte_cap;
  dst_batch->inst_count = src_batch->inst_count;
  dst_batch->inst_cap = src_batch->inst_cap;
  QueuePush(list->first, list->last, dst_batch);
  list->batch_count += 1;
  list->inst_count += dst_batch->inst_count;
  list->byte_count += dst_batch->byte_count;
 }
}
