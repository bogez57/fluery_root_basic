////////////////////////////////
//~ rjf: Globals

#if BUILD_CORE
B32 d_initialized = 0;
D_State *d_state = 0;
per_thread D_ThreadCtx *d_thread_ctx = 0;
#endif

////////////////////////////////
//~ rjf: Stack Implementation Helper Macros

#define D_StackPush(node_type, val_type, name, new_val) \
{\
Arena *arena = D_ActiveArena();\
D_Bucket *bucket = D_ActiveBucket();\
node_type *node_v = bucket->name##_free;\
if(node_v != 0) {StackPop(bucket->name##_free);}\
else {node_v = PushArray(arena, node_type, 1);}\
node_v->v = new_val;\
new_val = bucket->name##_stack_top->v;\
StackPush(bucket->name##_stack_top, node_v);\
bucket->current_stack_gen += 1;\
return new_val;\
}

#define D_StackPop(node_type, val_type, name) \
{\
val_type result = d_thread_ctx->name##_nil_stack_top.v;\
D_Bucket *bucket = D_ActiveBucket();\
if(bucket->name##_stack_top != &d_thread_ctx->name##_nil_stack_top)\
{\
node_type *node = bucket->name##_stack_top;\
result = node->v;\
StackPop(bucket->name##_stack_top);\
StackPush(bucket->name##_free, node);\
bucket->current_stack_gen += 1;\
}\
return result;\
}

#define D_StackTop(node_type, val_type, name) \
{\
D_Bucket *bucket = D_ActiveBucket();\
val_type result = bucket->name##_stack_top->v;\
return result;\
}

////////////////////////////////
//~ rjf: Generated Code

#include "draw/generated/draw.meta.c"

////////////////////////////////
//~ rjf: Layer Initialization

root_function D_InitReceipt
D_Init(R_InitReceipt r_init_receipt, F_InitReceipt f_init_receipt)
{
 if(IsMainThread() && d_initialized == 0)
 {
  d_initialized = 1;
  Arena *arena = ArenaAlloc(Gigabytes(8));
  d_state = PushArray(arena, D_State, 1);
  d_state->arena = arena;
  U32 white_data[] = {0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff};
  d_state->white_texture = R_Texture2DAlloc(V2S64(2, 2), R_Texture2DFormat_RGBA8, R_Texture2DKind_Static, &white_data[0]);
  D_InitThread();
 }
 D_InitReceipt result = {0};
 return result;
}

root_function void
D_InitThread(void)
{
 Arena *arena = ArenaAlloc(Megabytes(1));
 d_thread_ctx = PushArray(arena, D_ThreadCtx, 1);
 d_thread_ctx->arena = arena;
 d_thread_ctx->fallback_arena = ArenaAlloc(Megabytes(256));
 D_InitBucketStacks(&d_thread_ctx->fallback_bucket);
 d_thread_ctx->bucket_selection_fallback.arena = d_thread_ctx->fallback_arena;
 d_thread_ctx->bucket_selection_fallback.bucket = &d_thread_ctx->fallback_bucket;
 d_thread_ctx->bucket_selection_top = &d_thread_ctx->bucket_selection_fallback;
 D_InitThreadStackTops;
}

////////////////////////////////
//~ rjf: Frame Boundaries

root_function void
D_BeginFrame(void)
{
}

root_function void
D_EndFrame(void)
{
}

////////////////////////////////
//~ rjf: Bucket Creation, Selection, & Submission

root_function D_Bucket *
D_BucketMake(Arena *arena)
{
 D_Bucket *bucket = PushArray(arena, D_Bucket, 1);
 D_InitBucketStacks(bucket);
 return bucket;
}

root_function void
D_PushBucket(Arena *arena, D_Bucket *bucket)
{
 D_BucketSelectionNode *node = d_thread_ctx->bucket_selection_free;
 if(node != 0)
 {
  StackPop(d_thread_ctx->bucket_selection_free);
 }
 else
 {
  node = PushArray(d_thread_ctx->arena, D_BucketSelectionNode, 1);
 }
 node->arena = arena;
 node->bucket = bucket;
 StackPush(d_thread_ctx->bucket_selection_top, node);
}

root_function void
D_PopBucket(void)
{
 D_BucketSelectionNode *node = d_thread_ctx->bucket_selection_top;
 if(node != &d_thread_ctx->bucket_selection_fallback)
 {
  StackPop(d_thread_ctx->bucket_selection_top);
  StackPush(d_thread_ctx->bucket_selection_free, node);
 }
 if(d_thread_ctx->bucket_selection_top == &d_thread_ctx->bucket_selection_fallback)
 {
  ArenaClear(d_thread_ctx->fallback_arena);
  MemoryZeroStruct(&d_thread_ctx->fallback_bucket);
  D_InitBucketStacks(&d_thread_ctx->fallback_bucket);
 }
}

root_function Arena *
D_ActiveArena(void)
{
 return d_thread_ctx->bucket_selection_top->arena;
}

root_function D_Bucket *
D_ActiveBucket(void)
{
 return d_thread_ctx->bucket_selection_top->bucket;
}

root_function void
D_Submit(R_Handle window_r, D_Bucket *bucket)
{
 R_WindowSubmit(window_r, bucket->cmds);
}

////////////////////////////////
//~ rjf: Command Building Helpers

root_function R_Cmd *
D_PushCmd(R_CmdKind kind, R_Handle albedo_texture)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 R_CmdList *cmds = &bucket->cmds;
 R_Cmd *cmd = cmds->last ? &cmds->last->cmd : 0;
 if(cmd == 0 || cmd->kind != kind ||
    bucket->last_cmd_stack_gen != bucket->current_stack_gen ||
    (!R_HandleMatch(cmd->albedo_texture, albedo_texture) &&
     !R_HandleMatch(albedo_texture, d_state->white_texture)))
 {
  R_Cmd new_cmd = {0};
  new_cmd.kind = kind;
  new_cmd.albedo_texture = albedo_texture;
  new_cmd.albedo_texture_sample_kind = bucket->texture2d_sample_kind_stack_top->v;
  new_cmd.xform2d        = bucket->xform2d_stack_top->v;
  new_cmd.clip           = bucket->clip_stack_top->v;
  new_cmd.opacity        = bucket->opacity_stack_top->v;
  R_CmdNode *new_cmd_node = R_CmdListPush(arena, cmds, &new_cmd);
  cmd = &new_cmd_node->cmd;
 }
 bucket->last_cmd_stack_gen = bucket->current_stack_gen;
 return cmd;
}

////////////////////////////////
//~ rjf: Command Building

//- rjf: rectangles (2d)

root_function R_Rect2DInst *
D_Rect2D_(Rng2F32 rect, D_RectParams p)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 R_Handle albedo_texture = R_HandleIsZero(p.albedo_texture) ? d_state->white_texture : p.albedo_texture;
 R_Cmd *cmd = D_PushCmd(R_CmdKind_Rect2D, albedo_texture);
 R_Rect2DInst *result = (R_Rect2DInst *)R_CmdInstBatchListPush(arena, &cmd->batches, 64, sizeof(R_Rect2DInst));
 result->dst_rect = rect;
 result->src_rect = p.src_rect;
 result->colors[Corner_00] = result->colors[Corner_01] = result->colors[Corner_10] = result->colors[Corner_11] = p.color;
 result->corner_radii[Corner_00] = result->corner_radii[Corner_01] = result->corner_radii[Corner_10] = result->corner_radii[Corner_11] = p.radius;
 result->border_thickness = p.border_thickness;
 result->softness = p.soft;
 result->omit_texture = (F32)!!R_HandleIsZero(p.albedo_texture);
 return result;
}

//- rjf: text (2d)

root_function F32
D_Text2D(Vec2F32 position, F_Tag font, F32 size, Vec4F32 color, String8 string)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 ArenaTemp scratch = GetScratch(&arena, 1);
 F_Run run = F_RunFromFontSizeString(scratch.arena, font, size, string);
 Vec2F32 p = position;
 for(F_Piece *piece = run.first_piece; piece != 0; piece = piece->next)
 {
  Vec2F32 size = Dim2F32(piece->src_rect);
  Vec2F32 p0 = Add2F32(p, piece->offset);
  Vec2F32 p1 = Add2F32(p0, size);
  Rng2F32 rect = R2F32(p0, p1);
  D_Rect2D(rect, .texture = piece->texture, .src_rect = piece->src_rect, .color = color);
  p.x += piece->advance;
 }
 ReleaseScratch(scratch);
 F32 result = p.x - position.x;
 return result;
}

root_function F32
D_Text2DF(Vec2F32 position, F_Tag font, F32 size, Vec4F32 color, char *fmt, ...)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 ArenaTemp scratch = GetScratch(&arena, 1);
 va_list args;
 va_start(args, fmt);
 String8 string = PushStr8FV(scratch.arena, fmt, args);
 F32 result = D_Text2D(position, font, size, color, string);
 va_end(args);
 ReleaseScratch(scratch);
 return result;
}

//- rjf: passes (3d)

root_function void
D_BeginPass3D_(R_Pass3DParams p)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 R_Cmd *cmd = D_PushCmd(R_CmdKind_Pass3D, R_HandleZero());
 R_Pass3DParams *dst_params = (R_Pass3DParams *)R_CmdInstBatchListPush(arena, &cmd->batches, 1, sizeof(R_Pass3DParams));
 MemoryCopyStruct(dst_params, &p);
 D_Pass3DNode *pass_node = PushArray(arena, D_Pass3DNode, 1);
 pass_node->params = dst_params;
 StackPush(bucket->top_3d_pass, pass_node);
}

root_function void
D_EndPass3D(void)
{
 D_Bucket *bucket = D_ActiveBucket();
 StackPop(bucket->top_3d_pass);
}

//- rjf: sprites (3d)

root_function R_Sprite3DInst *
D_Sprite3D_(Vec3F32 pos, Mat4x4F32 xform, D_SpriteParams p)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 R_Sprite3DInst *result = 0;
 if(bucket->top_3d_pass != 0)
 {
  R_Pass3DParams *pass_params = bucket->top_3d_pass->params;
  R_Handle albedo_texture = R_HandleIsZero(p.albedo_texture) ? d_state->white_texture : p.albedo_texture;
  result = R_CmdInstBatchListPush(arena, &pass_params->sprites, 1024, sizeof(R_Sprite3DInst));
  result->pos.xyz = pos;
  result->pos.w = 1.f;
  result->xform = xform;
  result->src_rect = p.src_rect;
  result->colors[Corner_00] = result->colors[Corner_01] = result->colors[Corner_10] = result->colors[Corner_11] = p.color;
 }
 return result;
}

//- rjf: debug lines (3d)

root_function R_DebugLine3DInst *
D_DebugLine3D(Vec3F32 p0, Vec3F32 p1, Vec4F32 color0, Vec4F32 color1)
{
 Arena *arena = D_ActiveArena();
 D_Bucket *bucket = D_ActiveBucket();
 R_DebugLine3DInst *result = 0;
 if(bucket->top_3d_pass != 0)
 {
  R_Pass3DParams *pass_params = bucket->top_3d_pass->params;
  result = R_CmdInstBatchListPush(arena, &pass_params->debug_lines, 1024, sizeof(R_DebugLine3DInst));
  result->p0 = V4F32(p0.x, p0.y, p0.z, 1.f);
  result->p1 = V4F32(p1.x, p1.y, p1.z, 1.f);
  result->color0 = color0;
  result->color1 = color1;
 }
 return result;
}

root_function void
D_DebugCuboid3D(Rng3F32 rng, Vec4F32 color)
{
 D_DebugLine3D(V3F32(rng.min.x, rng.min.y, rng.min.z), V3F32(rng.min.x, rng.max.y, rng.min.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.min.y, rng.min.z), V3F32(rng.max.x, rng.min.y, rng.min.z), color, color);
 D_DebugLine3D(V3F32(rng.max.x, rng.min.y, rng.min.z), V3F32(rng.max.x, rng.max.y, rng.min.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.max.y, rng.min.z), V3F32(rng.max.x, rng.max.y, rng.min.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.min.y, rng.max.z), V3F32(rng.min.x, rng.max.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.min.y, rng.max.z), V3F32(rng.max.x, rng.min.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.max.x, rng.min.y, rng.max.z), V3F32(rng.max.x, rng.max.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.max.y, rng.max.z), V3F32(rng.max.x, rng.max.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.min.y, rng.min.z), V3F32(rng.min.x, rng.min.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.min.x, rng.max.y, rng.min.z), V3F32(rng.min.x, rng.max.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.max.x, rng.min.y, rng.min.z), V3F32(rng.max.x, rng.min.y, rng.max.z), color, color);
 D_DebugLine3D(V3F32(rng.max.x, rng.max.y, rng.min.z), V3F32(rng.max.x, rng.max.y, rng.max.z), color, color);
}

root_function void
D_DebugSphere3D(Vec3F32 center, F32 radius, Vec4F32 color)
{
 for(F32 p = 0; p <= 1.1f; p += 0.1f)
 {
  // rjf: x/y plane
  {
   Vec3F32 circle_center = center;
   circle_center.z += radius*CosF32(p*0.5f);
   Vec3F32 last_vertex = {0};
   F32 p_radius = SinF32(p*0.5f) * radius;
   for(F32 angle_p = 0; angle_p <= 1.1f; angle_p += 0.05f)
   {
    F32 angle = angle_p;
    Vec3F32 vertex = Add3F32(circle_center, V3F32(p_radius*CosF32(angle), p_radius*SinF32(angle), 0));
    if(angle_p > 0)
    {
     D_DebugLine3D(vertex, last_vertex, color, color);
    }
    last_vertex = vertex;
   }
  }
  
  // rjf: x/z plane
  {
   Vec3F32 circle_center = center;
   circle_center.y += radius*CosF32(p*0.5f);
   Vec3F32 last_vertex = {0};
   F32 p_radius = SinF32(p*0.5f) * radius;
   for(F32 angle_p = 0; angle_p <= 1.1f; angle_p += 0.05f)
   {
    F32 angle = angle_p;
    Vec3F32 vertex = Add3F32(circle_center, V3F32(p_radius*CosF32(angle), 0, p_radius*SinF32(angle)));
    if(angle_p > 0)
    {
     D_DebugLine3D(vertex, last_vertex, color, color);
    }
    last_vertex = vertex;
   }
  }
 }
}

//- rjf: joining many buckets

root_function void
D_BucketConcatInPlace(D_Bucket *to_push)
{
 D_Bucket *bucket = D_ActiveBucket();
 R_CmdListConcatInPlace(&bucket->cmds, &to_push->cmds);
}
