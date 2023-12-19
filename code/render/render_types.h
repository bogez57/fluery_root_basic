/* date = July 19th 2022 9:45 pm */

#ifndef RENDER_TYPES_H
#define RENDER_TYPES_H

////////////////////////////////
//~ rjf: Function/Global Macros

#if LANG_C
# define r_global extern
#else
# define r_global no_name_mangle
#endif
#define r_function no_name_mangle

////////////////////////////////
//~ rjf: Basic Types

typedef struct R_InitReceipt R_InitReceipt;
struct R_InitReceipt
{
 U64 u64[1];
};

////////////////////////////////
//~ rjf: Handle Types

typedef union R_Handle R_Handle;
union R_Handle
{
 U64 u64[4];
 U32 u32[8];
};

////////////////////////////////
//~ rjf: Blending Types

typedef enum R_BlendMode
{
 R_BlendMode_Normal,
 R_BlendMode_Additive,
 R_BlendMode_COUNT
}
R_BlendMode;

////////////////////////////////
//~ rjf: Texture Types

typedef enum R_Tex2DFormat
{
 R_Tex2DFormat_Null,
 R_Tex2DFormat_R8,
 R_Tex2DFormat_RGBA8,
 R_Tex2DFormat_COUNT
}
R_Tex2DFormat;

typedef enum R_Tex2DSampleKind
{
 R_Tex2DSampleKind_Nearest,
 R_Tex2DSampleKind_Linear,
 R_Tex2DSampleKind_COUNT
}
R_Tex2DSampleKind;

typedef enum R_Tex2DKind
{
 R_Tex2DKind_Static,
 R_Tex2DKind_Dynamic,
}
R_Tex2DKind;

typedef struct R_Slice2F32 R_Slice2F32;
struct R_Slice2F32
{
 R_Handle texture;
 Rng2F32 region;
};

////////////////////////////////
//~ rjf: Instance Types

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
 F32 shear;
 F32 _unused_[2];
};

typedef struct R_PointLight3DInst R_PointLight3DInst;
struct R_PointLight3DInst
{
 Vec4F32 pos;
 Vec4F32 color;
 F32 intensity;
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

////////////////////////////////
//~ rjf: Batch Types

typedef struct R_Batch R_Batch;
struct R_Batch
{
 R_Batch *next;
 U8 *v;
 U64 byte_count;
 U64 byte_cap;
 U64 inst_count;
 U64 inst_cap;
};

typedef struct R_BatchList R_BatchList;
struct R_BatchList
{
 R_Batch *first;
 R_Batch *last;
 U64 batch_count;
 U64 inst_count;
 U64 byte_count;
};

typedef struct R_BatchGroup2DParams R_BatchGroup2DParams;
struct R_BatchGroup2DParams
{
 R_Handle albedo_tex;
 R_Tex2DSampleKind albedo_tex_sample_kind;
 Mat3x3F32 xform2d;
 Rng2F32 clip;
 F32 transparency;
};

typedef struct R_BatchGroup2DNode R_BatchGroup2DNode;
struct R_BatchGroup2DNode
{
 R_BatchGroup2DNode *next;
 R_BatchList batches;
 R_BatchGroup2DParams params;
};

typedef struct R_BatchGroup2DList R_BatchGroup2DList;
struct R_BatchGroup2DList
{
 R_BatchGroup2DNode *first;
 R_BatchGroup2DNode *last;
 U64 count;
};

typedef struct R_BatchGroup3DParams R_BatchGroup3DParams;
struct R_BatchGroup3DParams
{
 R_Handle albedo_tex;
 R_Tex2DSampleKind albedo_tex_sample_kind;
 R_BlendMode blend_mode;
};

typedef struct R_BatchGroup3DNode R_BatchGroup3DNode;
struct R_BatchGroup3DNode
{
 R_BatchGroup3DNode *next;
 R_BatchList batches;
 R_BatchGroup3DParams params;
};

typedef struct R_BatchGroup3DList R_BatchGroup3DList;
struct R_BatchGroup3DList
{
 R_BatchGroup3DNode *first;
 R_BatchGroup3DNode *last;
 U64 count;
};

typedef struct R_BatchGroup3DSlot R_BatchGroup3DSlot;
struct R_BatchGroup3DSlot
{
 R_BatchGroup3DNode *first;
 R_BatchGroup3DNode *last;
};

typedef struct R_BatchGroup3DMap R_BatchGroup3DMap;
struct R_BatchGroup3DMap
{
 U64 slots_count;
 R_BatchGroup3DSlot *slots;
};

////////////////////////////////
//~ rjf: Pass Types

typedef enum R_PassKind
{
 R_PassKind_Null,
 R_PassKind_UI,
 R_PassKind_G0,
 R_PassKind_COUNT
}
R_PassKind;

typedef struct R_PassParams_UI R_PassParams_UI;
struct R_PassParams_UI
{
 Rng2F32 viewport;
 R_BatchGroup2DList rects;
};

typedef struct R_PassParams_G0 R_PassParams_G0;
struct R_PassParams_G0
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
 
 // rjf: keylight
 Vec4F32 keylight_color;
 Vec3F32 keylight_dir;
 
 // rjf: instance batches
 R_BatchGroup3DMap sprites;
 R_BatchList debug_lines;
 R_BatchList point_lights;
};

typedef struct R_Pass R_Pass;
struct R_Pass
{
 R_PassKind kind;
 union
 {
  void *params;
  R_PassParams_UI *params_ui;
  R_PassParams_G0 *params_g0;
 };
};

typedef struct R_PassNode R_PassNode;
struct R_PassNode
{
 R_PassNode *next;
 R_Pass v;
};

typedef struct R_PassList R_PassList;
struct R_PassList
{
 R_PassNode *first;
 R_PassNode *last;
 U64 count;
};

////////////////////////////////
//~ rjf: Handle Type Functions

root_function R_Handle R_HandleZero(void);
root_function B32 R_HandleMatch(R_Handle a, R_Handle b);
root_function B32 R_HandleIsZero(R_Handle handle);

////////////////////////////////
//~ rjf: Texture Type Functions

root_function U64 R_BytesPerPixelFromTex2DFormat(R_Tex2DFormat fmt);

////////////////////////////////
//~ rjf: Pass Building Helper Functions

//- rjf: pass list building
root_function R_Pass *R_PassListPush(Arena *arena, R_PassList *list, R_PassKind kind);
root_function void R_PassListConcatInPlace(R_PassList *dst, R_PassList *src);

//- rjf: batch list building
root_function void *R_BatchListPush(Arena *arena, R_BatchList *list, U64 cap, U64 instance_size);
#define R_BatchListPushStruct(arena, list, cap, type) (type *)R_BatchListPush((arena), (list), (cap), sizeof(type))
root_function void R_BatchListConcatInPlace(R_BatchList *list, R_BatchList *to_push);
root_function void R_BatchListConcatDeepCopy(Arena *arena, R_BatchList *list, R_BatchList *to_push);

#endif // RENDER_TYPES_H
