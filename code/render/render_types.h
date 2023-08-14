/* date = July 19th 2022 9:45 pm */

#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

////////////////////////////////
//~ rjf: Function/Global Macros

#if LANG_C
# define render_global extern
#else
# define render_global no_name_mangle
#endif
#define render_function no_name_mangle

////////////////////////////////
//~ rjf: Basic Types

typedef struct R_InitReceipt R_InitReceipt;
struct R_InitReceipt
{
 U64 u64[1];
};

typedef enum R_Texture2DFormat
{
 R_Texture2DFormat_Null,
 R_Texture2DFormat_R8,
 R_Texture2DFormat_RGBA8,
 R_Texture2DFormat_COUNT
}
R_Texture2DFormat;

typedef enum R_Texture2DSampleKind
{
 R_Texture2DSampleKind_Nearest,
 R_Texture2DSampleKind_Linear,
 R_Texture2DSampleKind_COUNT
}
R_Texture2DSampleKind;

typedef enum R_Texture2DKind
{
 R_Texture2DKind_Static,
 R_Texture2DKind_Dynamic,
}
R_Texture2DKind;

typedef union R_Handle R_Handle;
union R_Handle
{
 U64 u64[4];
 U32 u32[8];
};

typedef struct R_Slice2F32 R_Slice2F32;
struct R_Slice2F32
{
 R_Handle texture;
 Rng2F32 region;
};

////////////////////////////////
//~ rjf: Command Types

typedef enum R_CmdKind
{
 R_CmdKind_Null,
 R_CmdKind_Rect2D,
 R_CmdKind_Pass3D,
 R_CmdKind_COUNT
}
R_CmdKind;

typedef struct R_Rect2DInst R_Rect2DInst;
struct R_Rect2DInst
{
 Rng2F32 dst_rect;
 Rng2F32 src_rect;
 Vec4F32 colors[Corner_COUNT];
 F32 corner_radii[Corner_COUNT];
 F32 border_thickness;
 F32 softness;
 F32 omit_texture;
 F32 _unused_[1];
};

typedef struct R_Sprite3DInst R_Sprite3DInst;
struct R_Sprite3DInst
{
 Vec4F32 pos;
 Mat4x4F32 xform;
 Rng2F32 src_rect;
 Vec4F32 colors[Corner_COUNT];
 F32 omit_texture;
 F32 _unused_[3];
};

typedef struct R_DebugLine3DInst R_DebugLine3DInst;
struct R_DebugLine3DInst
{
 Vec4F32 p0;
 Vec4F32 p1;
 Vec4F32 color0;
 Vec4F32 color1;
};

typedef struct R_CmdInstBatch R_CmdInstBatch;
struct R_CmdInstBatch
{
 R_CmdInstBatch *next;
 U8 *v;
 U64 byte_count;
 U64 byte_cap;
 U64 inst_count;
 U64 inst_cap;
};

typedef struct R_CmdInstBatchList R_CmdInstBatchList;
struct R_CmdInstBatchList
{
 R_CmdInstBatch *first;
 R_CmdInstBatch *last;
 U64 batch_count;
 U64 inst_count;
 U64 byte_count;
};

typedef struct R_Pass3DParams R_Pass3DParams;
struct R_Pass3DParams
{
 // rjf: viewport
 Rng2F32 viewport;
 
 // rjf: near/far z planes
 F32 near_z;
 F32 far_z;
 
 // rjf: view/projection
 Mat4x4F32 xform_view;
 Mat4x4F32 xform_projection;
 
 // rjf: fog
 Vec4F32 fog_color;
 F32 pct_fog_per_unit;
 
 // rjf: instance batches
 R_CmdInstBatchList sprites;
 R_CmdInstBatchList debug_lines;
};

typedef struct R_Cmd R_Cmd;
struct R_Cmd
{
 // rjf: payload
 R_CmdKind kind;
 R_CmdInstBatchList batches;
 
 // rjf: parameters
 R_Handle albedo_texture;
 R_Texture2DSampleKind albedo_texture_sample_kind;
 Mat3x3F32 xform2d;
 Rng2F32 clip;
 F32 opacity;
};

typedef struct R_CmdNode R_CmdNode;
struct R_CmdNode
{
 R_CmdNode *next;
 R_Cmd cmd;
};

typedef struct R_CmdList R_CmdList;
struct R_CmdList
{
 R_CmdNode *first;
 R_CmdNode *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Basic Type Functions

root_function R_Handle R_HandleZero(void);
root_function B32 R_HandleMatch(R_Handle a, R_Handle b);
root_function B32 R_HandleIsZero(R_Handle handle);
root_function U64 R_BytesPerPixelFromTexture2DFormat(R_Texture2DFormat fmt);

////////////////////////////////
//~ rjf: Command Type Functions

//- rjf: command kind metadata
root_function U64 R_InstanceSizeFromCmdKind(R_CmdKind kind);

//- rjf: command instance batch lists
root_function void *R_CmdInstBatchListPush(Arena *arena, R_CmdInstBatchList *list, U64 cap, U64 instance_size);
root_function void R_CmdInstBatchListConcatInPlace(R_CmdInstBatchList *list, R_CmdInstBatchList *to_push);
root_function void R_CmdInstBatchListConcatDeepCopy(Arena *arena, R_CmdInstBatchList *list, R_CmdInstBatchList *to_push);

//- rjf: command lists
root_function R_CmdNode *R_CmdListPush(Arena *arena, R_CmdList *list, R_Cmd *cmd);
root_function void R_CmdListConcatInPlace(R_CmdList *list, R_CmdList *to_push);

#endif // RENDER_TYPES_H
