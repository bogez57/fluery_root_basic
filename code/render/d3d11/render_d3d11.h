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
 F32 alpha_test_min;
};

struct R_D3D11_CmdGlobals_PointLight3D
{
 Mat4x4F32 xform;
 Mat4x4F32 inverse_view_projection;
};

struct R_D3D11_CmdGlobals_CompositeUnlit3D
{
 Mat4x4F32 inverse_view_projection;
 Mat4x4F32 shadowmap_view_projection;
 Vec4F32 fog_color;
 F32 pct_fog_per_unit;
 F32 near_z;
 F32 far_z;
};

struct R_D3D11_CmdGlobals_CompositeLit3D
{
 Mat4x4F32 inverse_view_projection;
 Vec4F32 fog_color;
 F32 pct_fog_per_unit;
 F32 near_z;
 F32 far_z;
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

struct R_D3D11_Tex2D
{
 ID3D11Texture2D *texture;
 ID3D11ShaderResourceView *view;
 Vec2S32 size;
 R_Tex2DFormat format;
 R_Tex2DKind kind;
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
 ID3D11BlendState *main_blend_state;
 ID3D11BlendState *additive_blend_state;
 ID3D11SamplerState *nearest_sampler;
 ID3D11SamplerState *linear_sampler;
 ID3D11DepthStencilState *noop_depth_stencil_state;
 ID3D11DepthStencilState *plain_depth_stencil_state;
 ID3D11DepthStencilState *write_depth_stencil_state;
 ID3D11DepthStencilState *read_depth_stencil_state;
 
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
 ID3D11Buffer *light_sphere_vtx_buffer;
 ID3D11Buffer *light_sphere_idx_buffer;
 U64 light_sphere_vtx_count;
 U64 light_sphere_idx_count;
 
 //- rjf: overflow buffers
 Arena *overflow_arena;
 R_D3D11_OverflowBufferNode *first_overflow_buffer;
 R_D3D11_OverflowBufferNode *last_overflow_buffer;
};

typedef struct R_D3D11_WindowEquip R_D3D11_WindowEquip;
struct R_D3D11_WindowEquip
{
 // rjf: window swapchain/framebuffer
 IDXGISwapChain1 *swapchain;
 ID3D11Texture2D *framebuffer;
 ID3D11RenderTargetView *framebuffer_rtv;
 
 // rjf: g-buffer
 ID3D11Texture2D *gbuffer_color;
 ID3D11RenderTargetView *gbuffer_color_rtv;
 ID3D11ShaderResourceView *gbuffer_color_srv;
 ID3D11Texture2D *gbuffer_depth;
 ID3D11DepthStencilView *gbuffer_depth_dsv;
 ID3D11ShaderResourceView *gbuffer_depth_srv;
 
 // rjf: g-buffer scratch
 ID3D11Texture2D *gbuffer_scratch_color;
 ID3D11RenderTargetView *gbuffer_scratch_color_rtv;
 ID3D11ShaderResourceView *gbuffer_scratch_color_srv;
 
 // rjf: shadow maps
 Vec2S64 shadowmap_resolution;
 ID3D11Texture2D *shadowmap_depth;
 ID3D11DepthStencilView *shadowmap_depth_dsv;
 ID3D11ShaderResourceView *shadowmap_depth_srv;
 
 // rjf: last state
 Vec2S64 last_resolution;
};

////////////////////////////////
//~ rjf: Globals

r_global R_D3D11_State *r_d3d11_state;

////////////////////////////////
//~ rjf: Basic Helpers

r_function DXGI_FORMAT R_D3D11_DXGIFormatFromTexture2DFormat(R_Tex2DFormat fmt);

r_function R_D3D11_WindowEquip *R_D3D11_WindowEquipFromHandle(R_Handle handle);
r_function R_Handle R_D3D11_HandleFromWindowEquip(R_D3D11_WindowEquip *equip);

r_function R_D3D11_Tex2D R_D3D11_Tex2DFromHandle(R_Handle handle);
r_function R_Handle R_D3D11_HandleFromTexture2D(R_D3D11_Tex2D texture);

r_function R_D3D11_Buffer R_D3D11_BufferFromHandle(R_Handle handle);
r_function R_Handle R_D3D11_HandleFromBuffer(R_D3D11_Buffer buffer);

r_function void R_D3D11_BufferWriteString(ID3D11DeviceContext1 *device_ctx, ID3D11Buffer *buffer, String8 data);
r_function ID3D11Buffer *R_D3D11_InstanceBufferFromBatchList(R_BatchList *list);

r_function Mat4x4F32 R_D3D11_SampleChannelMapFromTexture2DFormat(R_Tex2DFormat fmt);

////////////////////////////////
//~ rjf: Backend Hooks

// NOTE(rjf): implementations of main rendering hooks

#endif // RENDER_D3D11_H
