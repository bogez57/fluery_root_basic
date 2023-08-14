root_function R_Texture2DSampleKind  D_PushTexture2DSampleKind(R_Texture2DSampleKind v)  D_StackPush(D_Texture2DSampleKindNode, R_Texture2DSampleKind, texture2d_sample_kind, v)
root_function Mat3x3F32        D_PushTransform2D(Mat3x3F32 v)                    D_StackPush(D_Transform2DNode, Mat3x3F32, xform2d, v)
root_function Rng2F32          D_PushClip(Rng2F32 v)                             D_StackPush(D_ClipNode, Rng2F32, clip, v)
root_function F32              D_PushOpacity(F32 v)                              D_StackPush(D_OpacityNode, F32, opacity, v)

root_function R_Texture2DSampleKind  D_PopTexture2DSampleKind(void)              D_StackPop(D_Texture2DSampleKindNode, R_Texture2DSampleKind, texture2d_sample_kind)
root_function Mat3x3F32        D_PopTransform2D(void)                            D_StackPop(D_Transform2DNode, Mat3x3F32, xform2d)
root_function Rng2F32          D_PopClip(void)                                   D_StackPop(D_ClipNode, Rng2F32, clip)
root_function F32              D_PopOpacity(void)                                D_StackPop(D_OpacityNode, F32, opacity)

root_function R_Texture2DSampleKind  D_TopTexture2DSampleKind(void)              D_StackTop(D_Texture2DSampleKindNode, R_Texture2DSampleKind, texture2d_sample_kind)
root_function Mat3x3F32        D_TopTransform2D(void)                            D_StackTop(D_Transform2DNode, Mat3x3F32, xform2d)
root_function Rng2F32          D_TopClip(void)                                   D_StackTop(D_ClipNode, Rng2F32, clip)
root_function F32              D_TopOpacity(void)                                D_StackTop(D_OpacityNode, F32, opacity)

