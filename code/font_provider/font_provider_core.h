/* date = March 17th 2022 10:38 pm */

#ifndef FONT_PROVIDER_CORE_H
#define FONT_PROVIDER_CORE_H

////////////////////////////////
//~ rjf: Function Macros

#define fp_function no_name_mangle

////////////////////////////////
//~ rjf: Types

typedef struct FP_InitReceipt FP_InitReceipt;
struct FP_InitReceipt
{
 U64 u64[1];
};

typedef struct FP_Handle FP_Handle;
struct FP_Handle
{
 U64 u64[2];
};

typedef struct FP_Metrics FP_Metrics;
struct FP_Metrics
{
 F32 line_gap;
 F32 ascent;
 F32 descent;
 F32 capital_height;
};

typedef struct FP_RasterResult FP_RasterResult;
struct FP_RasterResult
{
 Vec2S64 atlas_dim;
 void *atlas;
 F32 advance;
 F32 height;
};

////////////////////////////////
//~ rjf: Basic Handle Functions

root_function FP_Handle FP_HandleZero(void);
root_function B32 FP_HandleMatch(FP_Handle a, FP_Handle b);

////////////////////////////////
//~ rjf: Backend Abstraction Functions

fp_function FP_InitReceipt FP_Init(C_InitReceipt c_init);
fp_function FP_Handle FP_FontOpen(U128 hash);
fp_function void FP_FontClose(FP_Handle handle);
fp_function FP_Metrics FP_MetricsFromFont(FP_Handle handle, F32 size);
fp_function FP_RasterResult FP_Raster(Arena *arena, FP_Handle handle, F32 size, String8 string);

#endif // FONT_PROVIDER_CORE_H
