typedef struct D_Texture2DSampleKindNode D_Texture2DSampleKindNode; struct D_Texture2DSampleKindNode {D_Texture2DSampleKindNode *next; R_Texture2DSampleKind v;};
typedef struct D_Transform2DNode D_Transform2DNode; struct D_Transform2DNode {D_Transform2DNode *next; Mat3x3F32 v;};
typedef struct D_ClipNode D_ClipNode; struct D_ClipNode {D_ClipNode *next; Rng2F32 v;};
typedef struct D_OpacityNode D_OpacityNode; struct D_OpacityNode {D_OpacityNode *next; F32 v;};

#define D_DeclThreadStackTops \
struct\
{\
D_Texture2DSampleKindNode texture2d_sample_kind_nil_stack_top;\
D_Transform2DNode xform2d_nil_stack_top;\
D_ClipNode clip_nil_stack_top;\
D_OpacityNode opacity_nil_stack_top;\
}

#define D_InitThreadStackTops \
d_thread_ctx->texture2d_sample_kind_nil_stack_top.v = R_Texture2DSampleKind_Nearest;\
d_thread_ctx->xform2d_nil_stack_top.v = MakeMat3x3F32(1.f);\
d_thread_ctx->clip_nil_stack_top.v = R2F32(V2F32(0, 0), V2F32(0, 0));\
d_thread_ctx->opacity_nil_stack_top.v = 1.f;\


#define D_DeclBucketStacks \
struct\
{\
D_Texture2DSampleKindNode *texture2d_sample_kind_stack_top; D_Texture2DSampleKindNode *texture2d_sample_kind_free;\
D_Transform2DNode *xform2d_stack_top; D_Transform2DNode *xform2d_free;\
D_ClipNode *clip_stack_top; D_ClipNode *clip_free;\
D_OpacityNode *opacity_stack_top; D_OpacityNode *opacity_free;\
}

#define D_InitBucketStacks(b) \
(b)->texture2d_sample_kind_stack_top = &d_thread_ctx->texture2d_sample_kind_nil_stack_top;\
(b)->xform2d_stack_top = &d_thread_ctx->xform2d_nil_stack_top;\
(b)->clip_stack_top = &d_thread_ctx->clip_nil_stack_top;\
(b)->opacity_stack_top = &d_thread_ctx->opacity_nil_stack_top;\


#if 0
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
#endif

