typedef struct D_Texture2DSampleKindNode D_Texture2DSampleKindNode; struct D_Texture2DSampleKindNode {D_Texture2DSampleKindNode *next; R_Texture2DSampleKind v;};
typedef struct D_Transform2DNode D_Transform2DNode; struct D_Transform2DNode {D_Transform2DNode *next; Mat3x3F32 v;};
typedef struct D_View3DNode D_View3DNode; struct D_View3DNode {D_View3DNode *next; Mat4x4F32 v;};
typedef struct D_Projection3DNode D_Projection3DNode; struct D_Projection3DNode {D_Projection3DNode *next; Mat4x4F32 v;};
typedef struct D_ClipNode D_ClipNode; struct D_ClipNode {D_ClipNode *next; Rng2F32 v;};
typedef struct D_OpacityNode D_OpacityNode; struct D_OpacityNode {D_OpacityNode *next; F32 v;};
typedef struct D_FlagsNode D_FlagsNode; struct D_FlagsNode {D_FlagsNode *next; R_CmdFlags v;};

#define D_DeclThreadStackTops \
struct\
{\
D_Texture2DSampleKindNode texture2d_sample_kind_nil_stack_top;\
D_Transform2DNode xform2d_nil_stack_top;\
D_View3DNode view3d_nil_stack_top;\
D_Projection3DNode projection3d_nil_stack_top;\
D_ClipNode clip_nil_stack_top;\
D_OpacityNode opacity_nil_stack_top;\
D_FlagsNode flags_nil_stack_top;\
}

#define D_InitThreadStackTops \
d_thread_ctx->texture2d_sample_kind_nil_stack_top.v = R_Texture2DSampleKind_Nearest;\
d_thread_ctx->xform2d_nil_stack_top.v = MakeMat3x3F32(1.f);\
d_thread_ctx->view3d_nil_stack_top.v = MakeMat4x4F32(1.f);\
d_thread_ctx->projection3d_nil_stack_top.v = MakeMat4x4F32(1.f);\
d_thread_ctx->clip_nil_stack_top.v = R2F32(V2F32(0, 0), V2F32(0, 0));\
d_thread_ctx->opacity_nil_stack_top.v = 1.f;\
d_thread_ctx->flags_nil_stack_top.v = 0;\


#define D_DeclBucketStacks \
struct\
{\
D_Texture2DSampleKindNode *texture2d_sample_kind_stack_top; D_Texture2DSampleKindNode *texture2d_sample_kind_free;\
D_Transform2DNode *xform2d_stack_top; D_Transform2DNode *xform2d_free;\
D_View3DNode *view3d_stack_top; D_View3DNode *view3d_free;\
D_Projection3DNode *projection3d_stack_top; D_Projection3DNode *projection3d_free;\
D_ClipNode *clip_stack_top; D_ClipNode *clip_free;\
D_OpacityNode *opacity_stack_top; D_OpacityNode *opacity_free;\
D_FlagsNode *flags_stack_top; D_FlagsNode *flags_free;\
}

#define D_InitBucketStacks(b) \
b.texture2d_sample_kind_stack_top = &d_thread_ctx->texture2d_sample_kind_nil_stack_top;\
b.xform2d_stack_top = &d_thread_ctx->xform2d_nil_stack_top;\
b.view3d_stack_top = &d_thread_ctx->view3d_nil_stack_top;\
b.projection3d_stack_top = &d_thread_ctx->projection3d_nil_stack_top;\
b.clip_stack_top = &d_thread_ctx->clip_nil_stack_top;\
b.opacity_stack_top = &d_thread_ctx->opacity_nil_stack_top;\
b.flags_stack_top = &d_thread_ctx->flags_nil_stack_top;\


#if 0
core_function R_Texture2DSampleKind  D_PushTexture2DSampleKind(R_Texture2DSampleKind v);
core_function Mat3x3F32        D_PushTransform2D(Mat3x3F32 v);
core_function Mat4x4F32        D_PushView3D(Mat4x4F32 v);
core_function Mat4x4F32        D_PushProjection3D(Mat4x4F32 v);
core_function Rng2F32          D_PushClip(Rng2F32 v);
core_function F32              D_PushOpacity(F32 v);
core_function R_CmdFlags       D_PushFlags(R_CmdFlags v);

core_function R_Texture2DSampleKind  D_PopTexture2DSampleKind(void);
core_function Mat3x3F32        D_PopTransform2D(void);
core_function Mat4x4F32        D_PopView3D(void);
core_function Mat4x4F32        D_PopProjection3D(void);
core_function Rng2F32          D_PopClip(void);
core_function F32              D_PopOpacity(void);
core_function R_CmdFlags       D_PopFlags(void);

core_function R_Texture2DSampleKind  D_TopTexture2DSampleKind(void);
core_function Mat3x3F32        D_TopTransform2D(void);
core_function Mat4x4F32        D_TopView3D(void);
core_function Mat4x4F32        D_TopProjection3D(void);
core_function Rng2F32          D_TopClip(void);
core_function F32              D_TopOpacity(void);
core_function R_CmdFlags       D_TopFlags(void);

#define D_Texture2DSampleKind(v)  DeferLoop(D_PushTexture2DSampleKind(v), D_PopTexture2DSampleKind())
#define D_Transform2D(v)       DeferLoop(D_PushTransform2D(v), D_PopTransform2D())
#define D_View3D(v)            DeferLoop(D_PushView3D(v), D_PopView3D())
#define D_Projection3D(v)      DeferLoop(D_PushProjection3D(v), D_PopProjection3D())
#define D_Clip(v)              DeferLoop(D_PushClip(v), D_PopClip())
#define D_Opacity(v)           DeferLoop(D_PushOpacity(v), D_PopOpacity())
#define D_Flags(v)             DeferLoop(D_PushFlags(v), D_PopFlags())
#endif

