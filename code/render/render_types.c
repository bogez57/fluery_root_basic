////////////////////////////////
//~ rjf: Basic Type Functions

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

root_function U64
R_BytesPerPixelFromTexture2DFormat(R_Texture2DFormat fmt)
{
 U64 result = 0;
 switch(fmt)
 {
  default:
  case R_Texture2DFormat_R8:   {result = 1;}break;
  case R_Texture2DFormat_RGBA8:{result = 4;}break;
 }
 return result;
}

////////////////////////////////
//~ rjf: Command Type Functions

//- rjf: command kind metadata

root_function U64
R_InstanceSizeFromCmdKind(R_CmdKind kind)
{
 U64 result = 0;
 switch(kind)
 {
  default:break;
  case R_CmdKind_Rect2D:{result = sizeof(R_Rect2DInst);}break;
  case R_CmdKind_Pass3D:{result = sizeof(R_Pass3DParams);}break;
 }
 return result;
}

//- rjf: command instance batch lists

root_function void *
R_CmdInstBatchListPush(Arena *arena, R_CmdInstBatchList *list, U64 cap, U64 instance_size)
{
 void *result = 0;
 
 // rjf: obtain batch. three possibilities:
 // (a) no batch -> create batch
 // (b) batch full & can grow -> try to grow batch if it'd be contiguous on the same arena
 // (c) batch full & cannot grow -> create new batch
 R_CmdInstBatch *batch = list->last;
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
   batch = PushArray(arena, R_CmdInstBatch, 1);
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
R_CmdInstBatchListConcatInPlace(R_CmdInstBatchList *list, R_CmdInstBatchList *to_push)
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
R_CmdInstBatchListConcatDeepCopy(Arena *arena, R_CmdInstBatchList *list, R_CmdInstBatchList *to_push)
{
 for(R_CmdInstBatch *src_batch = to_push->first;
     src_batch != 0;
     src_batch = src_batch->next)
 {
  R_CmdInstBatch *dst_batch = PushArray(arena, R_CmdInstBatch, 1);
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

//- rjf: command lists

root_function R_CmdNode *
R_CmdListPush(Arena *arena, R_CmdList *list, R_Cmd *cmd)
{
 R_CmdNode *n = PushArray(arena, R_CmdNode, 1);
 n->cmd = *cmd;
 QueuePush(list->first, list->last, n);
 list->count += 1;
 return n;
}

root_function void
R_CmdListConcatInPlace(R_CmdList *list, R_CmdList *to_push)
{
 if(list->last && to_push->first)
 {
  list->last->next = to_push->first;
  list->last = to_push->last;
  list->count += to_push->count;
 }
 else
 {
  *list = *to_push;
 }
 MemoryZeroStruct(to_push);
}
