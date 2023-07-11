#ifndef RENDER_D3D11_H
#define RENDER_D3D11_H

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

////////////////////////////////
//~ rjf: Table Metadata Types

struct R_D3D11_CmdGlobalKindInfo
{
 U64 size;
};

struct R_D3D11_ShaderPairKindInfo
{
 String8 name;
 String8 shader_blob;
 D3D11_INPUT_ELEMENT_DESC *element_descriptions;
 U64 element_description_count;
};

////////////////////////////////
//~ rjf: Pipeline Data Types

struct R_D3D11_CmdGlobals_Rect2D
{
 Vec2F32 viewport_size;
 F32 opacity;
 F32 _padding0_;
 Mat4x4F32 albedo_sample_channel_map;
 Vec2F32 albedo_t2d_size;
 Vec2F32 _padding1_;
 Vec4F32 xform[3];
 Vec2F32 xform_scale;
};

struct R_D3D11_CmdGlobals_Sprite3D
{
 Mat4x4F32 xform;
 Mat4x4F32 albedo_sample_channel_map;
 Vec2F32 albedo_t2d_size;
};

struct R_D3D11_CmdGlobals_DebugLine3D
{
 Mat4x4F32 xform;
};

////////////////////////////////
//~ rjf: Generated Code

#include "generated/render_d3d11.meta.h"

////////////////////////////////
//~ rjf: Resource Types

struct R_D3D11_Texture2D
{
 ID3D11Texture2D *texture;
 ID3D11ShaderResourceView *view;
 Vec2S32 size;
 R_Texture2DFormat format;
 R_Texture2DKind kind;
};

struct R_D3D11_Buffer
{
 ID3D11Buffer *obj;
 U64 size;
};

////////////////////////////////
//~ rjf: Pipelines

struct R_D3D11_Pipeline
{
 ID3D11VertexShader *vshad;
 ID3D11PixelShader *pshad;
 ID3D11InputLayout *ilay;
 ID3D11Buffer *constants;
};

////////////////////////////////
//~ rjf: Main State Bundle

struct R_D3D11_OverflowBufferNode
{
 R_D3D11_OverflowBufferNode *next;
 ID3D11Buffer *buffer;
};

typedef struct R_D3D11_State R_D3D11_State;
struct R_D3D11_State
{
 //- rjf: top-level
 Arena *arena;
 OS_Handle device_mutex;
 
 //- rjf: base d3d11 objects
 ID3D11Device *base_device;
 ID3D11DeviceContext *base_device_ctx;
 ID3D11Device1 *device;
 ID3D11DeviceContext1 *device_ctx;
 IDXGIDevice1 *dxgi_device;
 IDXGIAdapter *dxgi_adapter;
 IDXGIFactory2 *dxgi_factory;
 ID3D11RasterizerState1 *rasterizer;
 ID3D11BlendState *blend_state;
 ID3D11SamplerState *nearest_sampler;
 ID3D11SamplerState *linear_sampler;
 ID3D11DepthStencilState *noop_depth_stencil_state;
 ID3D11DepthStencilState *plain_depth_stencil_state;
 ID3D11DepthStencilState *write_depth_stencil_state;
 
 //- rjf: global buffers
 ID3D11Buffer *cmd_global_buffer_table[R_D3D11_CmdGlobalKind_COUNT];
 
 //- rjf: shaders
 ID3D11InputLayout *ilay_table[R_D3D11_ShaderPairKind_COUNT];
 ID3D11VertexShader *vshad_table[R_D3D11_ShaderPairKind_COUNT];
 ID3D11PixelShader *pshad_table[R_D3D11_ShaderPairKind_COUNT];
 
 //- rjf: base resources
 ID3D11Buffer *scratch_buffer_64kb;
 ID3D11Buffer *scratch_buffer_1mb;
 ID3D11Buffer *scratch_buffer_2mb;
 ID3D11Buffer *scratch_buffer_4mb;
 ID3D11Buffer *scratch_buffer_8mb;
 R_Handle white_texture;
 R_Handle backup_texture;
 
 //- rjf: overflow buffers
 Arena *overflow_arena;
 R_D3D11_OverflowBufferNode *first_overflow_buffer;
 R_D3D11_OverflowBufferNode *last_overflow_buffer;
};

typedef struct R_D3D11_WindowEquip R_D3D11_WindowEquip;
struct R_D3D11_WindowEquip
{
 IDXGISwapChain1 *swapchain;
 ID3D11Texture2D *framebuffer;
 ID3D11RenderTargetView *framebuffer_view;
 ID3D11Texture2D *depth_buffer;
 ID3D11DepthStencilView *depth_buffer_view;
 Vec2S64 last_resolution;
};

////////////////////////////////
//~ rjf: Globals

render_global R_D3D11_State *r_d3d11_state;

////////////////////////////////
//~ rjf: Basic Helpers

render_function DXGI_FORMAT R_D3D11_DXGIFormatFromTexture2DFormat(R_Texture2DFormat fmt);

render_function R_D3D11_WindowEquip *R_D3D11_WindowEquipFromHandle(R_Handle handle);
render_function R_Handle R_D3D11_HandleFromWindowEquip(R_D3D11_WindowEquip *equip);

render_function R_D3D11_Texture2D R_D3D11_Texture2DFromHandle(R_Handle handle);
render_function R_Handle R_D3D11_HandleFromTexture2D(R_D3D11_Texture2D texture);

render_function R_D3D11_Buffer R_D3D11_BufferFromHandle(R_Handle handle);
render_function R_Handle R_D3D11_HandleFromBuffer(R_D3D11_Buffer buffer);

render_function void R_D3D11_WriteGPUBuffer(ID3D11DeviceContext1 *device_ctx, ID3D11Buffer *buffer, U64 offset, String8 data);
render_function ID3D11Buffer *R_D3D11_InstanceBufferFromCmd(R_Cmd *cmd);

render_function Mat4x4F32 R_D3D11_SampleChannelMapFromTexture2DFormat(R_Texture2DFormat fmt);

////////////////////////////////
//~ rjf: Backend Hooks

// NOTE(rjf): implementations of main rendering hooks

#endif // RENDER_D3D11_H
