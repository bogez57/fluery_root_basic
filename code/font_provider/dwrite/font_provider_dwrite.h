#ifndef FONT_PROVIDER_DWRITE_H
#define FONT_PROVIDER_DWRITE_H

#include <windows.h>
#include <dwrite_1.h>

#pragma comment(lib, "dwrite")

////////////////////////////////
//~ rjf: Font File Loader Types

typedef struct FP_DWrite_FontFileLoaderVTable FP_DWrite_FontFileLoaderVTable;
struct FP_DWrite_FontFileLoaderVTable
{
 HRESULT (*QueryInterface)(struct FP_DWrite_FontFileLoader *this_ptr, REFIID riid, void **ppvObject);
 ULONG (*AddRef)(void *this_ptr);
 ULONG (*Release)(void *this_ptr);
 HRESULT (*CreateStreamFromKey)(struct FP_DWrite_FontFileLoader *this_ptr, void *key, U32 key_size, IDWriteFontFileStream **out_stream);
};

typedef struct FP_DWrite_FontFileLoader FP_DWrite_FontFileLoader;
struct FP_DWrite_FontFileLoader
{
 FP_DWrite_FontFileLoaderVTable *lpVtbl;
};

////////////////////////////////
//~ rjf: Font File Stream Types

typedef struct FP_DWrite_FontFileStreamVTable FP_DWrite_FontFileStreamVTable;
struct FP_DWrite_FontFileStreamVTable
{
 HRESULT (*QueryInterface)(struct FP_DWrite_FontFileStream *this_ptr, REFIID riid, void **ppvObject);
 ULONG (*AddRef)(struct FP_DWrite_FontFileStream *this_ptr);
 ULONG (*Release)(struct FP_DWrite_FontFileStream *this_ptr);
 HRESULT (*ReadFileFragment)(struct FP_DWrite_FontFileStream *this_ptr, void **fragment_start, U64 off, U64 size, void **fragment_ctx_out);
 void (*ReleaseFileFragment)(struct FP_DWrite_FontFileStream *this_ptr, void *fragment_ctx);
 HRESULT (*GetFileSize)(struct FP_DWrite_FontFileStream *this_ptr, U64 *out_file_size);
 HRESULT (*GetLastWriteTime)(struct FP_DWrite_FontFileStream *this_ptr, U64 *out_time);
};

typedef struct FP_DWrite_FontFileStream FP_DWrite_FontFileStream;
struct FP_DWrite_FontFileStream
{
 FP_DWrite_FontFileStreamVTable *lpVtbl;
 FP_DWrite_FontFileStream *next;
 C_Scope *scope;
 U128 hash;
 U64 ref_count;
};

////////////////////////////////
//~ rjf: Font Types

typedef struct FP_DWrite_Font FP_DWrite_Font;
struct FP_DWrite_Font
{
 IDWriteFontFile *font_file;
 IDWriteFontFace *font_face;
};

////////////////////////////////
//~ rjf: Main State Bundle Type

typedef struct FP_DWrite_State FP_DWrite_State;
struct FP_DWrite_State
{
 Arena *arena;
 IDWriteFactory *dwrite_factory;
 IDWriteRenderingParams *dwrite_base_rendering_params;
 IDWriteRenderingParams *dwrite_rendering_params;
 IDWriteGdiInterop *dwrite_gdi_interop;
 FP_DWrite_FontFileStream *first_free_stream;
};

////////////////////////////////
//~ rjf: IUnknown Helper Implementations

function ULONG FP_DWrite_NoOpAddRef(void *this_ptr);
function ULONG FP_DWrite_NoOpRelease(void *this_ptr);

////////////////////////////////
//~ rjf: Font File Loader Interface Implementations

function HRESULT FP_DWrite_FontFileLoader_QueryInterface(FP_DWrite_FontFileLoader *this_ptr, REFIID riid, void **ppvObject);
function HRESULT FP_DWrite_FontFileLoader_CreateStreamFromKey(FP_DWrite_FontFileLoader *this_ptr, void *key, U32 key_size, IDWriteFontFileStream **out_stream);

////////////////////////////////
//~ rjf: Font File Stream Interface Implementations

function HRESULT FP_DWrite_FontFileStream_QueryInterface(FP_DWrite_FontFileStream *this_ptr, REFIID riid, void **ppvObject);
function ULONG FP_DWrite_FontFileStream_AddRef(FP_DWrite_FontFileStream *this_ptr);
function ULONG FP_DWrite_FontFileStream_Release(FP_DWrite_FontFileStream *this_ptr);
function HRESULT FP_DWrite_FontFileStream_ReadFileFragment(FP_DWrite_FontFileStream *this_ptr, void **fragment_start, U64 off, U64 size, void **fragment_ctx_out);
function void FP_DWrite_FontFileStream_ReleaseFileFragment(FP_DWrite_FontFileStream *this_ptr, void *fragment_ctx);
function HRESULT FP_DWrite_FontFileStream_GetFileSize(FP_DWrite_FontFileStream *this_ptr, U64 *out_file_size);
function HRESULT FP_DWrite_FontFileStream_GetLastWriteTime(FP_DWrite_FontFileStream *this_ptr, U64 *out_time);

////////////////////////////////
//~ rjf: Font Type Functions

function FP_DWrite_Font FP_DWrite_FontFromHandle(FP_Handle handle);
function FP_Handle FP_DWrite_HandleFromFont(FP_DWrite_Font font);

#endif // FONT_PROVIDER_DWRITE_H
