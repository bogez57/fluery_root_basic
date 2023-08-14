#ifndef DRAW_H
#define DRAW_H

////////////////////////////////
//~ rjf: Basics

typedef struct D_InitReceipt D_InitReceipt;
struct D_InitReceipt
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Generated Code

#include "draw/generated/draw.meta.h"

////////////////////////////////
//~ rjf: Command Types

typedef struct D_Bucket D_Bucket;
struct D_Bucket
{
 // rjf: render commands
 R_CmdList cmds;
 
 // rjf: 3d pass stack
 struct D_Pass3DNode *top_3d_pass;
 
 // rjf: parameter stacks
 U64 last_cmd_stack_gen;
 U64 current_stack_gen;
 D_DeclBucketStacks;
};

typedef struct D_Pass3DNode D_Pass3DNode;
struct D_Pass3DNode
{
 D_Pass3DNode *next;
 R_Pass3DParams *params;
};

////////////////////////////////
//~ rjf: Main & Per-Thread State Types

typedef struct D_State D_State;
struct D_State
{
 Arena *arena;
 R_Handle white_texture;
};

typedef struct D_BucketSelectionNode D_BucketSelectionNode;
struct D_BucketSelectionNode
{
 D_BucketSelectionNode *next;
 D_Bucket *bucket;
 Arena *arena;
};

typedef struct D_ThreadCtx D_ThreadCtx;
struct D_ThreadCtx
{
 Arena *arena;
 Arena *fallback_arena;
 D_Bucket fallback_bucket;
 D_BucketSelectionNode bucket_selection_fallback;
 D_BucketSelectionNode *bucket_selection_top;
 D_BucketSelectionNode *bucket_selection_free;
 D_DeclThreadStackTops;
};

////////////////////////////////
//~ rjf: Drawing Param Types

typedef struct D_RectParams D_RectParams;
struct D_RectParams
{
 Vec4F32 color;
 union
 {
  F32 corner_radius;
  F32 corner;
  F32 radius;
  F32 r;
 };
 union
 {
  F32 border_thickness;
  F32 border;
  F32 thickness;
  F32 t;
 };
 union
 {
  F32 softness;
  F32 soft;
  F32 s;
 };
 union
 {
  R_Slice2F32 slice;
  struct
  {
   union
   {
    R_Handle albedo_texture;
    R_Handle albedo;
    R_Handle texture;
    R_Handle tex;
   };
   union
   {
    Rng2F32 src_rect;
    Rng2F32 src;
   };
  };
 };
};

typedef struct D_SpriteParams D_SpriteParams;
struct D_SpriteParams
{
 Vec4F32 color;
 Vec2F32 scale;
 union
 {
  R_Slice2F32 slice;
  struct
  {
   union
   {
    R_Handle albedo_texture;
    R_Handle albedo;
    R_Handle texture;
    R_Handle tex;
   };
   union
   {
    Rng2F32 src_rect;
    Rng2F32 src;
   };
  };
 };
};

////////////////////////////////
//~ rjf: Globals

root_global B32 d_initialized;
root_global D_State *d_state;
extern per_thread D_ThreadCtx *d_thread_ctx;

////////////////////////////////
//~ rjf: Initialization

root_function D_InitReceipt D_Init(R_InitReceipt r_init_receipt, F_InitReceipt f_init_receipt);
root_function void D_InitThread(void);

////////////////////////////////
//~ rjf: Frame Boundaries

root_function void D_BeginFrame(void);
root_function void D_EndFrame(void);

////////////////////////////////
//~ rjf: Bucket Creation, Selection, & Submission

root_function D_Bucket *D_BucketMake(Arena *arena);
root_function void D_PushBucket(Arena *arena, D_Bucket *bucket);
root_function void D_PopBucket(void);
#define D_BucketScope(arena, bucket) DeferLoop(D_PushBucket((arena), (bucket)), D_PopBucket())
root_function Arena *D_ActiveArena(void);
root_function D_Bucket *D_ActiveBucket(void);
root_function void D_Submit(R_Handle window_r, D_Bucket *bucket);

////////////////////////////////
//~ rjf: Command Building Helpers

root_function R_Cmd *D_PushCmd(R_CmdKind kind, R_Handle albedo_texture);

////////////////////////////////
//~ rjf: Command Building

//- rjf: rectangles (2d)
#define D_Rect2D(r, ...) D_Rect2D_((r), (D_RectParams){.color = {1, 1, 1, 1}, __VA_ARGS__})
root_function R_Rect2DInst *D_Rect2D_(Rng2F32 rect, D_RectParams p);

//- rjf: text (2d)
root_function F32 D_Text2D(Vec2F32 position, F_Tag font, F32 size, Vec4F32 color, String8 string);
root_function F32 D_Text2DF(Vec2F32 position, F_Tag font, F32 size, Vec4F32 color, char *fmt, ...);

//- rjf: passes (3d)
#define D_Pass3D(viewport_, ...) DeferLoop(D_BeginPass3D_((R_Pass3DParams){.viewport = (viewport_), __VA_ARGS__}), D_EndPass3D())
#define D_BeginPass3D(viewport_, ...) D_BeginPass3D_((R_Pass3DParams){.viewport = (viewport_), __VA_ARGS__})
root_function void D_BeginPass3D_(R_Pass3DParams p);
root_function void D_EndPass3D(void);

//- rjf: sprites (3d)
#define D_Sprite3D(pos, xform, ...) D_Sprite3D_((pos), (xform), (D_SpriteParams){.color = {1, 1, 1, 1}, .scale = {1, 1}, __VA_ARGS__})
root_function R_Sprite3DInst *D_Sprite3D_(Vec3F32 pos, Mat4x4F32 xform, D_SpriteParams p);

//- rjf: debug lines (3d)
root_function R_DebugLine3DInst *D_DebugLine3D(Vec3F32 p0, Vec3F32 p1, Vec4F32 color0, Vec4F32 color1);
root_function void D_DebugCuboid3D(Rng3F32 rng, Vec4F32 color);
root_function void D_DebugSphere3D(Vec3F32 center, F32 radius, Vec4F32 color);

//- rjf: joining many buckets
root_function void D_BucketConcatInPlace(D_Bucket *to_push);

////////////////////////////////
//~ rjf: Stacks

root_function R_Texture2DSampleKind  D_PushTexture2DSampleKind(R_Texture2DSampleKind v);
root_function Mat3x3F32        D_PushTransform2D(Mat3x3F32 v);
root_function Rng2F32          D_PushClip(Rng2F32 v);
root_function F32              D_PushOpacity(F32 v);

root_function R_Texture2DSampleKind  D_PopTexture2DSampleKind(void);
root_function Mat3x3F32        D_PopTransform2D(void);
root_function Rng2F32          D_PopClip(void);
root_function F32              D_PopOpacity(void);

root_function R_Texture2DSampleKind  D_TopTexture2DSampleKind(void);
root_function Mat3x3F32        D_TopTransform2D(void);
root_function Rng2F32          D_TopClip(void);
root_function F32              D_TopOpacity(void);

#define D_Texture2DSampleKind(v)  DeferLoop(D_PushTexture2DSampleKind(v), D_PopTexture2DSampleKind())
#define D_Transform2D(v)       DeferLoop(D_PushTransform2D(v), D_PopTransform2D())
#define D_Clip(v)              DeferLoop(D_PushClip(v), D_PopClip())
#define D_Opacity(v)           DeferLoop(D_PushOpacity(v), D_PopOpacity())

#endif // DRAW_H
