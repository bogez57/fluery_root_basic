#ifndef RENDER_CORE_H
#define RENDER_CORE_H

////////////////////////////////
//~ rjf: Backend Abstraction

r_function R_InitReceipt R_Init(OS_InitReceipt os_init, OS_InitGfxReceipt os_gfx_init);
r_function R_Handle R_WindowEquip(OS_Handle window);
r_function void R_WindowUnequip(OS_Handle window, R_Handle window_eqp);
r_function R_Handle R_Tex2DAlloc(Vec2S64 size, R_Tex2DFormat fmt, R_Tex2DKind kind, void *initial_data);
r_function void R_Tex2DRelease(R_Handle texture);
r_function void R_Tex2DFillRegion(R_Handle texture, Rng2S64 region, void *data);
r_function Vec2S64 R_SizeFromTex2D(R_Handle texture);
r_function R_Tex2DFormat R_FormatFromTex2D(R_Handle texture);
r_function void R_BeginFrame(void);
r_function void R_EndFrame(void);
r_function void R_WindowStart(R_Handle window_eqp, Vec2S64 resolution);
r_function void R_WindowSubmit(R_Handle window_eqp, R_PassList *passes);
r_function void R_WindowFinish(R_Handle window_eqp);

#endif // RENDER_CORE_H
