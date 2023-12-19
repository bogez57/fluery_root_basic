//- GENERATED CODE

root_function R_Tex2DSampleKind          D_PushTex2DSampleKind(R_Tex2DSampleKind v)  D_StackPush(D_Tex2DSampleKindNode, R_Tex2DSampleKind, tex2d_sample_kind, v)
root_function R_BlendMode                D_PushBlendMode(R_BlendMode v)          D_StackPush(D_BlendModeNode, R_BlendMode, blend_mode, v)
root_function Mat3x3F32                  D_PushTransform2D(Mat3x3F32 v)          D_StackPush(D_Transform2DNode, Mat3x3F32, xform2d, v)
root_function Rng2F32                    D_PushClip(Rng2F32 v)                   D_StackPush(D_ClipNode, Rng2F32, clip, v)
root_function F32                        D_PushTransparency(F32 v)               D_StackPush(D_TransparencyNode, F32, transparency, v)

root_function R_Tex2DSampleKind          D_PopTex2DSampleKind(void)              D_StackPop(D_Tex2DSampleKindNode, R_Tex2DSampleKind, tex2d_sample_kind)
root_function R_BlendMode                D_PopBlendMode(void)                    D_StackPop(D_BlendModeNode, R_BlendMode, blend_mode)
root_function Mat3x3F32                  D_PopTransform2D(void)                  D_StackPop(D_Transform2DNode, Mat3x3F32, xform2d)
root_function Rng2F32                    D_PopClip(void)                         D_StackPop(D_ClipNode, Rng2F32, clip)
root_function F32                        D_PopTransparency(void)                 D_StackPop(D_TransparencyNode, F32, transparency)

root_function R_Tex2DSampleKind          D_TopTex2DSampleKind(void)              D_StackTop(D_Tex2DSampleKindNode, R_Tex2DSampleKind, tex2d_sample_kind)
root_function R_BlendMode                D_TopBlendMode(void)                    D_StackTop(D_BlendModeNode, R_BlendMode, blend_mode)
root_function Mat3x3F32                  D_TopTransform2D(void)                  D_StackTop(D_Transform2DNode, Mat3x3F32, xform2d)
root_function Rng2F32                    D_TopClip(void)                         D_StackTop(D_ClipNode, Rng2F32, clip)
root_function F32                        D_TopTransparency(void)                 D_StackTop(D_TransparencyNode, F32, transparency)
