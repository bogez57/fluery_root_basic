////////////////////////////////
//~ rjf: Basic Type Functions

core_function R_Handle
R_HandleZero(void)
{
 R_Handle result = {0};
 return result;
}

core_function B32
R_HandleMatch(R_Handle a, R_Handle b)
{
 return (a.u64[0] == b.u64[0] &&
         a.u64[1] == b.u64[1] &&
         a.u64[2] == b.u64[2] &&
         a.u64[3] == b.u64[3]);
}

core_function B32
R_HandleIsZero(R_Handle handle)
{
 return R_HandleMatch(handle, R_HandleZero());
}

core_function U64
R_BytesPerPixelFromTexture2DFormat(R_Texture2DFormat fmt)
{
 U64 result = 0;
 switch(fmt)
 {
  default:
  case R_Texture2DFormat_R8:    result = 1; break;
  case R_Texture2DFormat_RGBA8: result = 4; break;
 }
 return result;
}

////////////////////////////////
//~ rjf: Command Type Functions

core_function U64
R_InstanceSizeFromCmdKind(R_CmdKind kind)
{
 U64 result = 0;
 switch(kind)
 {
  default:break;
  case R_CmdKind_Rect2D:     {result = sizeof(R_Rect2DInst);}break;
  case R_CmdKind_Sprite3D:   {result = sizeof(R_Sprite3DInst);}break;
  case R_CmdKind_DebugLine3D:{result = sizeof(R_DebugLine3DInst);}break;
 }
 return result;
}

core_function void *
R_PushCmdInstance(Arena *arena, R_Cmd *cmd)
{
 // rjf: kind => metadata
 U64 size_per_instance = R_InstanceSizeFromCmdKind(cmd->kind);
 
 // rjf: grab current alloc ptr
 void *arena_top = ArenaPushNoZero(arena, 0);
 
 // rjf: grab current batch - push new one if we either can't
 // push the new instance onto the previous batch, or if we don't
 // have a batch yet
 R_CmdBatch *batch = cmd->last_batch;
 if(batch == 0 || (batch->ext != 0 && (U8 *)batch->ext + batch->instance_count*size_per_instance != arena_top))
 {
  batch = PushArray(arena, R_CmdBatch, 1);
  QueuePush(cmd->first_batch, cmd->last_batch, batch);
 }
 
 // rjf: allocate data for single instance
 void *result = PushArray(arena, U8, size_per_instance);
 
 // rjf: fill batch info
 if(batch->ext == 0)
 {
  batch->ext = result;
 }
 batch->instance_count += 1;
 cmd->total_instance_count += 1;
 
 return result;
}

core_function R_CmdNode *
R_CmdListPush(Arena *arena, R_CmdList *list, R_Cmd *cmd)
{
 R_CmdNode *n = PushArray(arena, R_CmdNode, 1);
 n->cmd = *cmd;
 QueuePush(list->first, list->last, n);
 list->count += 1;
 return n;
}

core_function void
R_CmdListJoin(R_CmdList *list, R_CmdList *to_push)
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

core_function void
R_DeepConcatCmd(Arena *arena, R_Cmd *dst_cmd, R_Cmd *src_cmd)
{
 dst_cmd->total_instance_count += src_cmd->total_instance_count;
 for(R_CmdBatch *src_batch = src_cmd->first_batch; src_batch != 0; src_batch = src_batch->next)
 {
  R_CmdBatch *dst_batch = PushArray(arena, R_CmdBatch, 1);
  dst_batch->instance_count = src_batch->instance_count;
  if(src_batch->ext != 0)
  {
   U64 ext_size = src_batch->instance_count * R_InstanceSizeFromCmdKind(src_cmd->kind);
   dst_batch->ext = PushArrayNoZero(arena, U8, ext_size);
   MemoryCopy(dst_batch->ext, src_batch->ext, ext_size);
  }
  QueuePush(dst_cmd->first_batch, dst_cmd->last_batch, dst_batch);
 }
}
