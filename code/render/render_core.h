#ifndef RENDER_CORE_H
#define RENDER_CORE_H

////////////////////////////////
//~ rjf: Backend Abstraction

render_function R_InitReceipt R_Init(OS_InitReceipt os_init, OS_InitGfxReceipt os_gfx_init);
render_function R_Handle R_WindowEquip(OS_Handle window);
render_function void R_WindowUnequip(OS_Handle window, R_Handle window_eqp);
render_function R_Handle R_Texture2DAlloc(Vec2S64 size, R_Texture2DFormat fmt, R_Texture2DKind kind, void *initial_data);
render_function void R_Texture2DRelease(R_Handle texture);
render_function void R_Texture2DFillRegion(R_Handle texture, Rng2S64 region, void *data);
render_function Vec2S64 R_SizeFromTexture2D(R_Handle texture);
render_function R_Texture2DFormat R_FormatFromTexture2D(R_Handle texture);
render_function void R_BeginFrame(void);
render_function void R_EndFrame(void);
render_function void R_WindowStart(R_Handle window_eqp, Vec2S64 resolution);
render_function void R_WindowSubmit(R_Handle window_eqp, R_CmdList commands);
render_function void R_WindowFinish(R_Handle window_eqp);

#endif // RENDER_CORE_H
