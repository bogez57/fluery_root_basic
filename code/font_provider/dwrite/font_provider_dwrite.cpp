// NOTE(rjf): 1000x thanks to Allen Webster for making his example at
// https://github.com/4th-dimention/examps/tree/master/win32-direct-write.
// This API would've been mind-numbing to crack without it.

////////////////////////////////
//~ rjf: Globals

global FP_DWrite_State *fp_dwrite_state = 0;
read_only global FP_DWrite_FontFileLoaderVTable fp_g_dwrite_font_file_loader_vtbl =
{
 FP_DWrite_FontFileLoader_QueryInterface,
 FP_DWrite_NoOpAddRef,
 FP_DWrite_NoOpRelease,
 FP_DWrite_FontFileLoader_CreateStreamFromKey,
};
read_only global FP_DWrite_FontFileStreamVTable fp_g_dwrite_font_file_stream_vtbl =
{
 FP_DWrite_FontFileStream_QueryInterface,
 FP_DWrite_FontFileStream_AddRef,
 FP_DWrite_FontFileStream_Release,
 FP_DWrite_FontFileStream_ReadFileFragment,
 FP_DWrite_FontFileStream_ReleaseFileFragment,
 FP_DWrite_FontFileStream_GetFileSize,
 FP_DWrite_FontFileStream_GetLastWriteTime,
};
global FP_DWrite_FontFileLoader fp_g_dwrite_font_file_loader = {&fp_g_dwrite_font_file_loader_vtbl};

////////////////////////////////
//~ rjf: IUnknown Helper Implementations

function ULONG
FP_DWrite_NoOpAddRef(void *this_ptr)
{
 return 1;
}

function ULONG
FP_DWrite_NoOpRelease(void *this_ptr)
{
 return 1;
}

////////////////////////////////
//~ rjf: Font File Loader Interface Implementations

function HRESULT
FP_DWrite_FontFileLoader_QueryInterface(FP_DWrite_FontFileLoader *this_ptr, REFIID riid, void **ppvObject)
{
 HRESULT result = S_OK;
 *ppvObject = &fp_g_dwrite_font_file_loader;
 return result;
}

function HRESULT
FP_DWrite_FontFileLoader_CreateStreamFromKey(FP_DWrite_FontFileLoader *this_ptr, void *key, U32 key_size, IDWriteFontFileStream **out_stream)
{
 HRESULT result = S_OK;
 {
  Assert(key_size == sizeof(U128));
  FP_DWrite_FontFileStream *stream = fp_dwrite_state->first_free_stream;
  if(stream == 0)
  {
   stream = PushArrayNoZero(fp_dwrite_state->arena, FP_DWrite_FontFileStream, 1);
  }
  else
  {
   StackPop(fp_dwrite_state->first_free_stream);
  }
  MemoryZeroStruct(stream);
  stream->lpVtbl = &fp_g_dwrite_font_file_stream_vtbl;
  stream->scope = C_ScopeOpen();
  MemoryCopy(&stream->hash, key, sizeof(U128));
  stream->ref_count = 1;
  *out_stream = (IDWriteFontFileStream *)stream;
 }
 return result;
}

////////////////////////////////
//~ rjf: Font File Stream Interface Implementations

function HRESULT
FP_DWrite_FontFileStream_QueryInterface(FP_DWrite_FontFileStream *this_ptr, REFIID riid, void **ppvObject)
{
 HRESULT result = S_OK;
 *ppvObject = this_ptr;
 return result;
}

function ULONG
FP_DWrite_FontFileStream_AddRef(FP_DWrite_FontFileStream *this_ptr)
{
 this_ptr->ref_count += 1;
 return this_ptr->ref_count;
}

function ULONG
FP_DWrite_FontFileStream_Release(FP_DWrite_FontFileStream *this_ptr)
{
 this_ptr->ref_count -= 1;
 if(this_ptr->ref_count == 0)
 {
  C_ScopeClose(this_ptr->scope);
  StackPush(fp_dwrite_state->first_free_stream, this_ptr);
 }
 return this_ptr->ref_count;
}

function HRESULT
FP_DWrite_FontFileStream_ReadFileFragment(FP_DWrite_FontFileStream *this_ptr, void **fragment_start, U64 off, U64 size, void **fragment_ctx_out)
{
 HRESULT result = S_OK;
 {
  String8 data = C_DataFromHash(this_ptr->scope, this_ptr->hash);
  *fragment_start = data.str + off;
  *fragment_ctx_out = 0;
 }
 return result;
}

function void
FP_DWrite_FontFileStream_ReleaseFileFragment(FP_DWrite_FontFileStream *this_ptr, void *fragment_ctx)
{
 // NOTE(rjf): no-op
}

function HRESULT
FP_DWrite_FontFileStream_GetFileSize(FP_DWrite_FontFileStream *this_ptr, U64 *out_file_size)
{
 HRESULT result = S_OK;
 {
  String8 data = C_DataFromHash(this_ptr->scope, this_ptr->hash);
  *out_file_size = data.size;
 }
 return result;
}

function HRESULT
FP_DWrite_FontFileStream_GetLastWriteTime(FP_DWrite_FontFileStream *this_ptr, U64 *out_time)
{
 HRESULT result = S_OK;
 *out_time = 0;
 return result;
}

////////////////////////////////
//~ rjf: Font Type Functions

function FP_DWrite_Font
FP_DWrite_FontFromHandle(FP_Handle handle)
{
 FP_DWrite_Font result = {0};
 result.font_file = (IDWriteFontFile *)handle.u64[0];
 result.font_face = (IDWriteFontFace *)handle.u64[1];
 return result;
}

function FP_Handle
FP_DWrite_HandleFromFont(FP_DWrite_Font font)
{
 FP_Handle handle = {0};
 handle.u64[0] = (U64)font.font_file;
 handle.u64[1] = (U64)font.font_face;
 return handle;
}

////////////////////////////////
//~ rjf: Hooks

fp_function FP_InitReceipt
FP_Init(C_InitReceipt c_init)
{
 if(IsMainThread() && fp_dwrite_state == 0)
 {
  HRESULT error = 0;
  
  //- rjf: allocate/prep global state
  Arena *arena = ArenaAlloc(Gigabytes(4));
  fp_dwrite_state = PushArray(arena, FP_DWrite_State, 1);
  fp_dwrite_state->arena = arena;
  
  //- rjf: make dwrite factory
  error = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), (IUnknown**)&fp_dwrite_state->dwrite_factory);
  
  //- rjf: register font file loader
  error = fp_dwrite_state->dwrite_factory->RegisterFontFileLoader((IDWriteFontFileLoader *)&fp_g_dwrite_font_file_loader);
  
  //- rjf: make dwrite base rendering params
  error = fp_dwrite_state->dwrite_factory->CreateRenderingParams(&fp_dwrite_state->dwrite_base_rendering_params);
  
  //- rjf: make dwrite rendering params
  {
   FLOAT gamma = fp_dwrite_state->dwrite_base_rendering_params->GetGamma();
   FLOAT enhanced_contrast = fp_dwrite_state->dwrite_base_rendering_params->GetEnhancedContrast();
   FLOAT clear_type_level = fp_dwrite_state->dwrite_base_rendering_params->GetClearTypeLevel();
   error = fp_dwrite_state->dwrite_factory->CreateCustomRenderingParams(gamma,
                                                                        enhanced_contrast,
                                                                        clear_type_level,
                                                                        DWRITE_PIXEL_GEOMETRY_FLAT,
                                                                        DWRITE_RENDERING_MODE_DEFAULT,
                                                                        &fp_dwrite_state->dwrite_rendering_params);
  }
  
  //- rjf: setup dwrite/gdi interop
  error = fp_dwrite_state->dwrite_factory->GetGdiInterop(&fp_dwrite_state->dwrite_gdi_interop);
 }
 FP_InitReceipt receipt = {0};
 return receipt;
}

fp_function FP_Handle
FP_FontOpen(U128 hash)
{
 HRESULT error = 0;
 
 //- rjf: make a "font file reference"... oh boy x2...
 IDWriteFontFile *font_file = 0;
 error = fp_dwrite_state->dwrite_factory->CreateCustomFontFileReference(&hash, sizeof(hash), (IDWriteFontFileLoader*)&fp_g_dwrite_font_file_loader, &font_file);
 
 //- rjf: make dwrite font face
 IDWriteFontFace *font_face = 0;
 error = fp_dwrite_state->dwrite_factory->CreateFontFace(DWRITE_FONT_FACE_TYPE_UNKNOWN, 1, &font_file, 0, DWRITE_FONT_SIMULATIONS_NONE, &font_face);
 
 //- rjf: fill font
 FP_DWrite_Font font = {0};
 {
  font.font_file = font_file;
  font.font_face = font_face;
 };
 
 //- rjf: fill & return
 FP_Handle result = FP_DWrite_HandleFromFont(font);
 return result;
}

fp_function void
FP_FontClose(FP_Handle handle)
{
 FP_DWrite_Font font = FP_DWrite_FontFromHandle(handle);
 font.font_face->Release();
 font.font_file->Release();
}

fp_function FP_Metrics
FP_MetricsFromFont(FP_Handle handle, F32 size)
{
 FP_DWrite_Font font = FP_DWrite_FontFromHandle(handle);
 
 //- rjf: get font metrics
 DWRITE_FONT_METRICS font_metrics = {0};
 if(font.font_face != 0)
 {
  font.font_face->GetMetrics(&font_metrics);
 }
 
 //- rjf: fill & return
 FP_Metrics result = {0};
 if(font.font_face != 0)
 {
  F32 design_units_per_em = (F32)font_metrics.designUnitsPerEm;
  result.line_gap       = (96.f/72.f)*size * (F32)font_metrics.lineGap / design_units_per_em;
  result.ascent         = (96.f/72.f)*size * (F32)font_metrics.ascent / design_units_per_em;
  result.descent        = (96.f/72.f)*size * (F32)font_metrics.descent / design_units_per_em;
  result.capital_height = (96.f/72.f)*size * (F32)font_metrics.capHeight / design_units_per_em;
 }
 
 return result;
}

fp_function FP_RasterResult
FP_Raster(Arena *arena, FP_Handle handle, F32 size, String8 string)
{
 Temp scratch = ScratchBegin(&arena, 1);
 HRESULT error = 0;
 String32 str32 = Str32From8(scratch.arena, string);
 FP_DWrite_Font font = FP_DWrite_FontFromHandle(handle);
 COLORREF bg_color = RGB(0, 0, 0);
 COLORREF fg_color = RGB(255, 255, 255);
 
 //- rjf: get font metrics
 DWRITE_FONT_METRICS font_metrics = {0};
 if(font.font_face != 0)
 {
  font.font_face->GetMetrics(&font_metrics);
 }
 F32 design_units_per_em = (F32)font_metrics.designUnitsPerEm;
 
 //- rjf: get glyph indices
 U16 *glyph_indices = PushArrayNoZero(scratch.arena, U16, str32.size);
 if(font.font_face != 0)
 {
  error = font.font_face->GetGlyphIndices(str32.str, str32.size, glyph_indices);
 }
 
 //- rjf: get metrics info
 U64 glyphs_count = str32.size;
 DWRITE_GLYPH_METRICS *glyphs_metrics = PushArrayNoZero(scratch.arena, DWRITE_GLYPH_METRICS, glyphs_count);
 if(font.font_face != 0)
 {
  error = font.font_face->GetGdiCompatibleGlyphMetrics(size, 1.f, 0, 1, glyph_indices, glyphs_count, glyphs_metrics, 0);
 }
 
 //- rjf: derive info from metrics
 F32 advance = 0;
 Vec2S64 atlas_dim = {0};
 if(font.font_face != 0)
 {
  atlas_dim.y = (S64)((96.f/72.f) * size * (font_metrics.ascent + font_metrics.descent) / design_units_per_em);
  for(U64 idx = 0; idx < glyphs_count; idx += 1)
  {
   DWRITE_GLYPH_METRICS *glyph_metrics = glyphs_metrics + idx;
   F32 glyph_advance_width  = (96.f/72.f) * size * glyph_metrics->advanceWidth       / design_units_per_em;
   F32 glyph_advance_height = (96.f/72.f) * size * glyph_metrics->advanceHeight      / design_units_per_em;
   advance += (S64)glyph_advance_width;
   atlas_dim.x = (S64)Max((F32)atlas_dim.x, advance);
  }
  atlas_dim.x += 7;
  atlas_dim.x -= atlas_dim.x%8;
  atlas_dim.x += 4;
  atlas_dim.y += 2;
 }
 
 //- rjf: make dwrite bitmap for rendering
 IDWriteBitmapRenderTarget *render_target = 0;
 if(font.font_face != 0)
 {
  error = fp_dwrite_state->dwrite_gdi_interop->CreateBitmapRenderTarget(0, atlas_dim.x, atlas_dim.y, &render_target);
  render_target->SetPixelsPerDip(1.0);
 }
 
 //- rjf: get bitmap & clear
 HDC dc = 0;
 if(font.font_face != 0)
 {
  dc = render_target->GetMemoryDC();
  HGDIOBJ original = SelectObject(dc, GetStockObject(DC_PEN));
  SetDCPenColor(dc, bg_color);
  SelectObject(dc, GetStockObject(DC_BRUSH));
  SetDCBrushColor(dc, bg_color);
  Rectangle(dc, 0, 0, atlas_dim.x, atlas_dim.y);
  SelectObject(dc, original);
 }
 
 //- rjf: draw glyph run
 Vec2F32 draw_p = {1, (F32)atlas_dim.y - 2.f};
 if(font.font_face != 0)
 {
  F32 ascent = (96.f/72.f)*size * font_metrics.ascent / design_units_per_em;
  F32 descent = (96.f/72.f)*size * font_metrics.descent / design_units_per_em;
  draw_p.y -= descent;
 }
 DWRITE_GLYPH_RUN glyph_run = {0};
 if(font.font_face != 0)
 {
  glyph_run.fontFace = font.font_face;
  glyph_run.fontEmSize = size * 96.f/72.f;
  glyph_run.glyphCount = str32.size;
  glyph_run.glyphIndices = glyph_indices;
 }
 RECT bounding_box = {0};
 if(font.font_face != 0)
 {
  error = render_target->DrawGlyphRun(draw_p.x, draw_p.y,
                                      DWRITE_MEASURING_MODE_NATURAL,
                                      &glyph_run,
                                      fp_dwrite_state->dwrite_rendering_params,
                                      fg_color,
                                      &bounding_box);
 }
 
 //- rjf: get bitmap
 DIBSECTION dib = {0};
 if(font.font_face != 0)
 {
  HBITMAP bitmap = (HBITMAP)GetCurrentObject(dc, OBJ_BITMAP);
  GetObject(bitmap, sizeof(dib), &dib);
 }
 
 //- rjf: fill & return
 FP_RasterResult result = {0};
 if(font.font_face != 0)
 {
  // rjf: fill basics
  result.atlas_dim   = atlas_dim;
  result.atlas       = PushArrayNoZero(arena, U8, atlas_dim.x*atlas_dim.y*4);
  result.advance     = advance;
  result.height      = bounding_box.bottom + 4.f;
  
  // rjf: fill atlas
  {
   U8 *in_data   = (U8 *)dib.dsBm.bmBits;
   U64 in_pitch  = (U64)dib.dsBm.bmWidthBytes;
   U8 *out_data  = (U8 *)result.atlas;
   U64 out_pitch = atlas_dim.x * 4;
   
   U8 *in_line = (U8 *)in_data;
   U8 *out_line = out_data;
   for(U64 y = 0; y < atlas_dim.y; y += 1)
   {
    U8 *in_pixel = in_line;
    U8 *out_pixel = out_line;
    for(U64 x = 0; x < atlas_dim.x; x += 1)
    {
     out_pixel[0] = 255;
     out_pixel[1] = 255;
     out_pixel[2] = 255;
     out_pixel[3] = in_pixel[0];
     in_pixel += 4;
     out_pixel += 4;
    }
    in_line += in_pitch;
    out_line += out_pitch;
   }
  }
  render_target->Release();
 }
 ScratchEnd(scratch);
 return result;
}
