#define BUILD_ROOT 0
#define OS_FEATURE_GFX 1

#include "base/base_inc.h"
#include "os/os_inc.h"
#include "render/render_types.h"
#include "render/render_core.h"

#include "render_d3d11.h"

#include "base/base_inc.c"
#include "os/os_inc.c"
#include "render/render_types.c"
#include "render/render_core.c"

////////////////////////////////
//~ rjf: Input Layout Tables

global D3D11_INPUT_ELEMENT_DESC r_d3d11_g_rect2d_ilay_elements[] =
{
 { "POS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "TEX",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "CRAD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "STY",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

global D3D11_INPUT_ELEMENT_DESC r_d3d11_g_sprite3d_ilay_elements[] =
{
 { "POS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "XFM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "XFM",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "XFM",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "XFM",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "TEX",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "OTX",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

global D3D11_INPUT_ELEMENT_DESC r_d3d11_g_debugline3d_ilay_elements[] =
{
 { "POS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,                            0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "POS",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

////////////////////////////////
//~ rjf: Generated Code

#include "generated/render_d3d11.meta.c"

////////////////////////////////
//~ rjf: Globals

R_D3D11_State *r_d3d11_state = 0;

////////////////////////////////
//~ rjf: Basic Helpers

render_function DXGI_FORMAT
R_D3D11_DXGIFormatFromTexture2DFormat(R_Texture2DFormat fmt)
{
 DXGI_FORMAT result = DXGI_FORMAT_R8G8B8A8_UNORM;
 switch(fmt)
 {
  case R_Texture2DFormat_R8:
  {
   result = DXGI_FORMAT_R8_UNORM;
  }break;
  
  default:
  case R_Texture2DFormat_RGBA8: {}break;
 }
 return result;
}

render_function R_D3D11_WindowEquip *
R_D3D11_WindowEquipFromHandle(R_Handle handle)
{
 R_D3D11_WindowEquip *result = (R_D3D11_WindowEquip *)handle.u64[0];
 return result;
}

render_function R_Handle
R_D3D11_HandleFromWindowEquip(R_D3D11_WindowEquip *equip)
{
 R_Handle h = {0};
 h.u64[0] = (U64)equip;
 return h;
}

render_function R_D3D11_Texture2D
R_D3D11_Texture2DFromHandle(R_Handle handle)
{
 R_D3D11_Texture2D texture = {0};
 texture.texture = (ID3D11Texture2D *)handle.u64[0];
 texture.view = (ID3D11ShaderResourceView *)handle.u64[1];
 texture.size.x = handle.u32[4];
 texture.size.y = handle.u32[5];
 texture.format = (R_Texture2DFormat)handle.u32[6];
 texture.kind = (R_Texture2DKind)handle.u32[7];
 return texture;
}

render_function R_Handle
R_D3D11_HandleFromTexture2D(R_D3D11_Texture2D texture)
{
 R_Handle result = {0};
 result.u64[0] = (U64)texture.texture;
 result.u64[1] = (U64)texture.view;
 result.u32[4] = texture.size.x;
 result.u32[5] = texture.size.y;
 result.u32[6] = texture.format;
 result.u32[7] = texture.kind;
 return result;
}

render_function R_D3D11_Buffer
R_D3D11_BufferFromHandle(R_Handle handle)
{
 R_D3D11_Buffer buffer = {0};
 buffer.obj = (ID3D11Buffer *)handle.u64[0];
 buffer.size = handle.u64[1];
 return buffer;
}

render_function R_Handle
R_D3D11_HandleFromBuffer(R_D3D11_Buffer buffer)
{
 R_Handle handle = {0};
 handle.u64[0] = (U64)buffer.obj;
 handle.u64[1] = buffer.size;
 return handle;
}

render_function void
R_D3D11_WriteGPUBuffer(ID3D11DeviceContext1 *device_ctx, ID3D11Buffer *buffer, U64 offset, String8 data)
{
 D3D11_MAPPED_SUBRESOURCE sub_rsrc = {0};
 device_ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub_rsrc);
 U8 *ptr = (U8 *)sub_rsrc.pData + offset;
 MemoryCopy(ptr, data.str, data.size);
 device_ctx->Unmap(buffer, 0);
}

render_function ID3D11Buffer *
R_D3D11_InstanceBufferFromCmd(R_Cmd *cmd)
{
 U64 size_per_instance = R_InstanceSizeFromCmdKind(cmd->kind);
 U64 instance_count = cmd->total_instance_count;
 U64 needed_size = size_per_instance*instance_count;
 
 // rjf: pick buffer
 ID3D11Buffer *buffer = r_d3d11_state->scratch_buffer_8mb;
 {
  if(0){}
  else if(needed_size <= Kilobytes(64)) { buffer = r_d3d11_state->scratch_buffer_64kb; }
  else if(needed_size <= Megabytes(1))  { buffer = r_d3d11_state->scratch_buffer_1mb; }
  else if(needed_size <= Megabytes(2))  { buffer = r_d3d11_state->scratch_buffer_2mb; }
  else if(needed_size <= Megabytes(4))  { buffer = r_d3d11_state->scratch_buffer_4mb; }
  else if(needed_size <= Megabytes(8))  { buffer = r_d3d11_state->scratch_buffer_8mb; }
  else
  {
   R_D3D11_OverflowBufferNode *node = PushArray(r_d3d11_state->overflow_arena, R_D3D11_OverflowBufferNode, 1);
   D3D11_BUFFER_DESC desc = {0};
   {
    desc.ByteWidth      = needed_size;
    desc.Usage          = D3D11_USAGE_DYNAMIC;
    desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   }
   r_d3d11_state->device->CreateBuffer(&desc, 0, &node->buffer);
   QueuePush(r_d3d11_state->first_overflow_buffer, r_d3d11_state->last_overflow_buffer, node);
   buffer = node->buffer;
  }
 }
 
 // rjf: upload instance data
 {
  ArenaTemp scratch = GetScratch(0, 0);
  String8List strs = {0};
  for(R_CmdBatch *batch = cmd->first_batch; batch != 0; batch = batch->next)
  {
   String8 data = Str8((U8 *)batch->ext, batch->instance_count*size_per_instance);
   Str8ListPush(scratch.arena, &strs, data);
  }
  String8 data = Str8ListJoin(scratch.arena, strs, 0);
  R_D3D11_WriteGPUBuffer(r_d3d11_state->device_ctx, buffer, 0, data);
  ReleaseScratch(scratch);
 }
 
 return buffer;
}

render_function Mat4x4F32
R_D3D11_SampleChannelMapFromTexture2DFormat(R_Texture2DFormat fmt)
{
 Mat4x4F32 map = MakeMat4x4F32(1.f);
 switch(fmt)
 {
  default: break;
  case R_Texture2DFormat_R8:
  {
   map =
   {
    {
     { 1, 1, 1, 1 },
     { 0, 0, 0, 0 },
     { 0, 0, 0, 0 },
     { 0, 0, 0, 0 },
    },
   };
  }break;
 }
 return map;
}

////////////////////////////////
//~ rjf: Backend Hooks

render_function R_InitReceipt
R_Init(OS_InitReceipt os_init, OS_InitGfxReceipt os_gfx_init)
{
 if(IsMainThread() && r_d3d11_state == 0)
 {
  Arena *arena = ArenaAlloc(Gigabytes(16));
  r_d3d11_state = PushArray(arena, R_D3D11_State, 1);
  r_d3d11_state->arena = arena;
  r_d3d11_state->device_mutex = OS_MutexAlloc();
  
  //- rjf: initialize base device
  UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if BUILD_DEBUG
  creation_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
  D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0 };
  D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, creation_flags, feature_levels, ArrayCount(feature_levels), D3D11_SDK_VERSION, &r_d3d11_state->base_device, 0, &r_d3d11_state->base_device_ctx);
  
  //- randy: debug break
#if BUILD_DEBUG
  ID3D11InfoQueue *info = 0;
  r_d3d11_state->base_device->QueryInterface(__uuidof(ID3D11InfoQueue), (void **)(&info));
  info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
  info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
  info->Release();
#endif
  
  //- rjf: initialize device
  r_d3d11_state->base_device->QueryInterface(__uuidof(ID3D11Device1), (void **)(&r_d3d11_state->device));
  r_d3d11_state->base_device_ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)(&r_d3d11_state->device_ctx));
  
  //- rjf: initialize dxdgi device/adapter/factory
  r_d3d11_state->device->QueryInterface(__uuidof(IDXGIDevice1), (void **)(&r_d3d11_state->dxgi_device));
  r_d3d11_state->dxgi_device->GetAdapter(&r_d3d11_state->dxgi_adapter);
  r_d3d11_state->dxgi_adapter->GetParent(__uuidof(IDXGIFactory2), (void **)(&r_d3d11_state->dxgi_factory));
  
  //- rjf: make rasterizer
  {
   D3D11_RASTERIZER_DESC1 desc = {};
   {
    desc.FillMode = D3D11_FILL_SOLID;
    desc.CullMode = D3D11_CULL_NONE;
    desc.ScissorEnable = 1;
   }
   r_d3d11_state->device->CreateRasterizerState1(&desc, &r_d3d11_state->rasterizer);
  }
  
  //- rjf: make blend state
  {
   D3D11_BLEND_DESC desc = {};
   {
    desc.RenderTarget[0].BlendEnable            = 1;
    desc.RenderTarget[0].SrcBlend               = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend              = D3D11_BLEND_INV_SRC_ALPHA; 
    desc.RenderTarget[0].BlendOp                = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha          = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha         = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].BlendOpAlpha           = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;
   }
   r_d3d11_state->device->CreateBlendState(&desc, &r_d3d11_state->blend_state);
  }
  
  //- rjf: make samplers
  {
   {
    D3D11_SAMPLER_DESC desc = {};
    {
     desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_POINT;
     desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
     desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
     desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
     desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    }
    r_d3d11_state->device->CreateSamplerState(&desc, &r_d3d11_state->nearest_sampler);
   }
   {
    D3D11_SAMPLER_DESC desc = {};
    {
     desc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
     desc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
     desc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
     desc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
     desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    }
    r_d3d11_state->device->CreateSamplerState(&desc, &r_d3d11_state->linear_sampler);
   }
  }
  
  //- rjf: make depth/stencil states
  {
   {
    D3D11_DEPTH_STENCIL_DESC desc = {};
    {
     desc.DepthEnable    = FALSE;
     desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
     desc.DepthFunc      = D3D11_COMPARISON_LESS;
    }
    r_d3d11_state->device->CreateDepthStencilState(&desc, &r_d3d11_state->noop_depth_stencil_state);
   }
   {
    D3D11_DEPTH_STENCIL_DESC desc = {};
    {
     desc.DepthEnable    = TRUE;
     desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
     desc.DepthFunc      = D3D11_COMPARISON_LESS;
    }
    r_d3d11_state->device->CreateDepthStencilState(&desc, &r_d3d11_state->plain_depth_stencil_state);
   }
   {
    D3D11_DEPTH_STENCIL_DESC desc = {};
    {
     desc.DepthEnable    = FALSE;
     desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
     desc.DepthFunc      = D3D11_COMPARISON_LESS;
    }
    r_d3d11_state->device->CreateDepthStencilState(&desc, &r_d3d11_state->write_depth_stencil_state);
   }
  }
  
  //- rjf: create cmd global buffers
  for(R_D3D11_CmdGlobalKind kind = (R_D3D11_CmdGlobalKind)(R_D3D11_CmdGlobalKind_Nil+1);
      kind < R_D3D11_CmdGlobalKind_COUNT;
      kind = (R_D3D11_CmdGlobalKind)(kind+1))
  {
   D3D11_BUFFER_DESC desc = {0};
   {
    desc.ByteWidth = r_d3d11_g_cmd_global_kind_info_table[kind].size;
    desc.ByteWidth += 15;
    desc.ByteWidth -= desc.ByteWidth % 16;
    desc.Usage          = D3D11_USAGE_DYNAMIC;
    desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
   }
   r_d3d11_state->device->CreateBuffer(&desc, 0, &r_d3d11_state->cmd_global_buffer_table[kind]);
  }
  
  //- rjf: create shaders objects
  for(R_D3D11_ShaderPairKind kind = (R_D3D11_ShaderPairKind)(R_D3D11_ShaderPairKind_Nil+1);
      kind < R_D3D11_ShaderPairKind_COUNT;
      kind = (R_D3D11_ShaderPairKind)(kind+1))
  {
   R_D3D11_ShaderPairKindInfo *info = &r_d3d11_g_shader_pair_kind_info_table[kind];
   
   // rjf: compile vertex shader
   ID3DBlob *vshad_source_blob = 0;
   ID3DBlob *vshad_source_errors = 0;
   ID3D11VertexShader *vshad = 0;
   String8 vshad_errors = {0};
   {
    D3DCompile(info->shader_blob.str, info->shader_blob.size, (char *)info->name.str, 0, 0, "vs_main", "vs_5_0", 0, 0,
               &vshad_source_blob, &vshad_source_errors);
    if(vshad_source_errors)
    {
     vshad_errors = Str8((U8 *)vshad_source_errors->GetBufferPointer(),
                         (U64)vshad_source_errors->GetBufferSize());
    }
    else
    {
     r_d3d11_state->device->CreateVertexShader(vshad_source_blob->GetBufferPointer(), vshad_source_blob->GetBufferSize(), 0, &vshad);
    }
   }
   
   // rjf: make input layout
   ID3D11InputLayout *ilay = 0;
   {
    r_d3d11_state->device->CreateInputLayout(info->element_descriptions, info->element_description_count,
                                             vshad_source_blob->GetBufferPointer(),
                                             vshad_source_blob->GetBufferSize(),
                                             &ilay);
   }
   
   // rjf: compile pixel shader
   ID3DBlob *pshad_source_blob = 0;
   ID3DBlob *pshad_source_errors = 0;
   ID3D11PixelShader *pshad = 0;
   String8 pshad_errors = {0};
   {
    D3DCompile(info->shader_blob.str, info->shader_blob.size, (char *)info->name.str, 0, 0, "ps_main", "ps_5_0", 0, 0,
               &pshad_source_blob, &pshad_source_errors);
    if(pshad_source_errors)
    {
     pshad_errors = Str8((U8 *)pshad_source_errors->GetBufferPointer(),
                         (U64)pshad_source_errors->GetBufferSize());
    }
    else
    {
     r_d3d11_state->device->CreatePixelShader(pshad_source_blob->GetBufferPointer(), pshad_source_blob->GetBufferSize(), 0, &pshad);
    }
   }
   
   // rjf: store
   r_d3d11_state->ilay_table[kind] = ilay;
   r_d3d11_state->vshad_table[kind] = vshad;
   r_d3d11_state->pshad_table[kind] = pshad;
  }
  
  //- rjf: initialize base resources
  {
   // rjf: 64kb scratch
   {
    D3D11_BUFFER_DESC desc = {0};
    {
     desc.ByteWidth      = Kilobytes(64);
     desc.Usage          = D3D11_USAGE_DYNAMIC;
     desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
     desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    r_d3d11_state->device->CreateBuffer(&desc, 0, &r_d3d11_state->scratch_buffer_64kb);
   }
   
   // rjf: 1mb scratch
   {
    D3D11_BUFFER_DESC desc = {0};
    {
     desc.ByteWidth      = Megabytes(1);
     desc.Usage          = D3D11_USAGE_DYNAMIC;
     desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
     desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    r_d3d11_state->device->CreateBuffer(&desc, 0, &r_d3d11_state->scratch_buffer_1mb);
   }
   
   // rjf: 2mb scratch
   {
    D3D11_BUFFER_DESC desc = {0};
    {
     desc.ByteWidth      = Megabytes(2);
     desc.Usage          = D3D11_USAGE_DYNAMIC;
     desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
     desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    r_d3d11_state->device->CreateBuffer(&desc, 0, &r_d3d11_state->scratch_buffer_2mb);
   }
   
   // rjf: 4mb scratch
   {
    D3D11_BUFFER_DESC desc = {0};
    {
     desc.ByteWidth      = Megabytes(4);
     desc.Usage          = D3D11_USAGE_DYNAMIC;
     desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
     desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    r_d3d11_state->device->CreateBuffer(&desc, 0, &r_d3d11_state->scratch_buffer_4mb);
   }
   
   // rjf: 8mb scratch
   {
    D3D11_BUFFER_DESC desc = {0};
    {
     desc.ByteWidth      = Megabytes(8);
     desc.Usage          = D3D11_USAGE_DYNAMIC;
     desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
     desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    r_d3d11_state->device->CreateBuffer(&desc, 0, &r_d3d11_state->scratch_buffer_8mb);
   }
   
   // rjf: initialize white texture
   U32 white_texture_data = 0xffffffff;
   r_d3d11_state->white_texture = R_Texture2DAlloc(V2S64(1, 1), R_Texture2DFormat_RGBA8, R_Texture2DKind_Static, &white_texture_data);
   
   // rjf: initialize backup texture
   U8 backup_texture_data[] =
   {
    0xff, 0x00, 0xff, 0xff,
    0x11, 0x00, 0x11, 0xff,
    0x11, 0x00, 0x11, 0xff,
    0xff, 0x00, 0xff, 0xff,
   };
   r_d3d11_state->backup_texture = R_Texture2DAlloc(V2S64(2, 2), R_Texture2DFormat_RGBA8, R_Texture2DKind_Static, &backup_texture_data[0]);
  }
  
  //- rjf: set up overflow state
  r_d3d11_state->overflow_arena = ArenaAlloc(Kilobytes(256));
 }
 R_InitReceipt result = {0};
 return result;
}

render_function R_Handle
R_WindowEquip(OS_Handle window_handle)
{
 R_D3D11_WindowEquip *eqp = (R_D3D11_WindowEquip *)OS_Reserve(sizeof(R_D3D11_WindowEquip));
 OS_Commit(eqp, sizeof(*eqp));
 OS_W32_Window *window = OS_W32_WindowFromHandle(window_handle);
 HWND hwnd = window->hwnd;
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  //- rjf: initialize swapchain
  DXGI_SWAP_CHAIN_DESC1 swapchain_desc = {0};
  {
   swapchain_desc.Width              = 0; // NOTE(rjf): use window width
   swapchain_desc.Height             = 0; // NOTE(rjf): use window height
   swapchain_desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
   swapchain_desc.Stereo             = FALSE;
   swapchain_desc.SampleDesc.Count   = 1;
   swapchain_desc.SampleDesc.Quality = 0;
   swapchain_desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
   swapchain_desc.BufferCount        = 2;
   swapchain_desc.Scaling            = DXGI_SCALING_STRETCH;
   swapchain_desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
   swapchain_desc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
   swapchain_desc.Flags              = 0;
  }
  r_d3d11_state->dxgi_factory->CreateSwapChainForHwnd(r_d3d11_state->device, hwnd, &swapchain_desc, 0, 0, &eqp->swapchain);
  
  //- rjf: initialize framebuffer
  eqp->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)(&eqp->framebuffer));
  r_d3d11_state->device->CreateRenderTargetView(eqp->framebuffer, 0, &eqp->framebuffer_view);
 }
 return R_D3D11_HandleFromWindowEquip(eqp);
}

render_function void
R_WindowUnequip(OS_Handle window, R_Handle window_eqp)
{
 R_D3D11_WindowEquip *eqp = R_D3D11_WindowEquipFromHandle(window_eqp);
 eqp->framebuffer_view->Release();
 eqp->framebuffer->Release();
 eqp->swapchain->Release();
 if(eqp->depth_buffer)
 {
  eqp->depth_buffer->Release();
 }
 if(eqp->depth_buffer_view)
 {
  eqp->depth_buffer_view->Release();
 }
 OS_Release(eqp, sizeof(*eqp));
}

render_function R_Handle
R_Texture2DAlloc(Vec2S64 size, R_Texture2DFormat fmt, R_Texture2DKind kind, void *initial_data)
{
 // rjf: kind => usage
 D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
 switch(kind)
 {
  default:
  case R_Texture2DKind_Static: {usage = D3D11_USAGE_DYNAMIC;}break;
  case R_Texture2DKind_Dynamic:{usage = D3D11_USAGE_DEFAULT;}break;
 }
 D3D11_TEXTURE2D_DESC texture_desc = {};
 {
  texture_desc.Width              = size.x;
  texture_desc.Height             = size.y;
  texture_desc.MipLevels          = 1;
  texture_desc.ArraySize          = 1;
  texture_desc.Format             = R_D3D11_DXGIFormatFromTexture2DFormat(fmt);
  texture_desc.SampleDesc.Count   = 1;
  texture_desc.Usage              = usage;
  texture_desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
  texture_desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
 }
 D3D11_SUBRESOURCE_DATA initial_subrsrc_data = {};
 {
  initial_subrsrc_data.pSysMem = initial_data;
  initial_subrsrc_data.SysMemPitch = R_BytesPerPixelFromTexture2DFormat(fmt) * size.x;
  initial_subrsrc_data.SysMemSlicePitch = 0;
 }
 R_D3D11_Texture2D texture = {0};
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  r_d3d11_state->device->CreateTexture2D(&texture_desc, initial_data ? &initial_subrsrc_data : 0, &texture.texture);
  r_d3d11_state->device->CreateShaderResourceView(texture.texture, 0, &texture.view);
 }
 texture.size = Vec2S32FromVec(size);
 texture.format = fmt;
 return R_D3D11_HandleFromTexture2D(texture);
}

render_function void
R_Texture2DRelease(R_Handle texture_handle)
{
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  R_D3D11_Texture2D texture = R_D3D11_Texture2DFromHandle(texture_handle);
  texture.texture->Release();
  texture.view->Release();
 }
}

render_function void
R_Texture2DFillRegion(R_Handle texture_handle, Rng2S64 region, void *data)
{
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  R_D3D11_Texture2D tex = R_D3D11_Texture2DFromHandle(texture_handle);
  U64 bytes_per_pixel = R_BytesPerPixelFromTexture2DFormat(tex.format);
  Vec2S64 dim = Dim2S64(region);
  D3D11_BOX dst_box =
  {
   (UINT)region.x0, (UINT)region.y0, 0,
   (UINT)region.x1, (UINT)region.y1, 1,
  };
  r_d3d11_state->device_ctx->UpdateSubresource(tex.texture, 0, &dst_box, data, dim.x*bytes_per_pixel, 0);
 }
}

render_function Vec2S64
R_SizeFromTexture2D(R_Handle texture_handle)
{
 R_D3D11_Texture2D tex = R_D3D11_Texture2DFromHandle(texture_handle);
 return Vec2S64FromVec(tex.size);
}

render_function R_Texture2DFormat
R_FormatFromTexture2D(R_Handle texture_handle)
{
 R_D3D11_Texture2D tex = R_D3D11_Texture2DFromHandle(texture_handle);
 return tex.format;
}

render_function void
R_BeginFrame(void)
{
 
}

render_function void
R_EndFrame(void)
{
 for(R_D3D11_OverflowBufferNode *node = r_d3d11_state->first_overflow_buffer;
     node != 0;
     node = node->next)
 {
  node->buffer->Release();
 }
 ArenaClear(r_d3d11_state->overflow_arena);
 r_d3d11_state->first_overflow_buffer = r_d3d11_state->last_overflow_buffer = 0;
}

render_function void
R_WindowStart(R_Handle window_eqp, Vec2S64 resolution)
{
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  R_D3D11_WindowEquip *wnd = R_D3D11_WindowEquipFromHandle(window_eqp);
  ID3D11Device1 *device = r_d3d11_state->device;
  ID3D11DeviceContext1 *d_ctx = r_d3d11_state->device_ctx;
  IDXGISwapChain1 *swapchain = wnd->swapchain;
  ID3D11Texture2D *framebuffer = wnd->framebuffer;
  ID3D11RenderTargetView *framebuffer_view = wnd->framebuffer_view;
  
  //- rjf: resolution changed
  B32 resolution_changed = (wnd->last_resolution.x != resolution.x ||
                            wnd->last_resolution.y != resolution.y);
  wnd->last_resolution = resolution;
  
  //- rjf: set up swapchain
  if(resolution_changed)
  {
   framebuffer_view->Release();
   framebuffer->Release();
   swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
   swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)(&framebuffer));
   r_d3d11_state->device->CreateRenderTargetView(framebuffer, 0, &framebuffer_view);
   wnd->framebuffer = framebuffer;
   wnd->framebuffer_view = framebuffer_view;
  }
  
  //- rjf: set up depth/stencil buffer
  if(resolution_changed)
  {
   if(wnd->depth_buffer)
   {
    wnd->depth_buffer->Release();
   }
   if(wnd->depth_buffer_view)
   {
    wnd->depth_buffer_view->Release();
   }
   D3D11_TEXTURE2D_DESC depth_buffer_desc = {};
   {
    wnd->framebuffer->GetDesc(&depth_buffer_desc);
    depth_buffer_desc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
   }
   device->CreateTexture2D(&depth_buffer_desc, 0, &wnd->depth_buffer);
   device->CreateDepthStencilView(wnd->depth_buffer, 0, &wnd->depth_buffer_view);
  }
  
  //- rjf: clear framebuffer
  Vec4F32 clear_color = {0};
  d_ctx->ClearRenderTargetView(framebuffer_view, clear_color.v);
  d_ctx->ClearDepthStencilView(wnd->depth_buffer_view, D3D11_CLEAR_DEPTH, 1.f, 0);
 }
}

render_function void
R_WindowSubmit(R_Handle window_eqp, R_CmdList commands)
{
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  R_D3D11_WindowEquip *wnd = R_D3D11_WindowEquipFromHandle(window_eqp);
  ID3D11DeviceContext1 *d_ctx = r_d3d11_state->device_ctx;
  IDXGISwapChain1 *swapchain = wnd->swapchain;
  ID3D11Texture2D *framebuffer = wnd->framebuffer;
  ID3D11RenderTargetView *framebuffer_view = wnd->framebuffer_view;
  ID3D11DepthStencilView *depth_buffer_view = wnd->depth_buffer_view;
  ID3D11RasterizerState *rasterizer = r_d3d11_state->rasterizer;
  
  //- rjf: do commands
  R_CmdFlags last_flags = 0;
  Rng2F32 last_viewport = {0};
  for(R_CmdNode *cmd_node = commands.first;
      cmd_node != 0;
      cmd_node = cmd_node->next)
  {
   //- rjf: unpack command
   R_Cmd *cmd = &cmd_node->cmd;
   R_CmdFlags new_flags = cmd->flags;
   B32 flags_change = (new_flags != last_flags || cmd_node == commands.first);
   last_flags = new_flags;
   Rng2F32 viewport = cmd->viewport;
   B32 viewport_change = (cmd_node == commands.first ||
                          viewport.x0 != last_viewport.x0 ||
                          viewport.y0 != last_viewport.y0 ||
                          viewport.x1 != last_viewport.x1 ||
                          viewport.y1 != last_viewport.y1);
   last_viewport = viewport;
   U64 bytes_per_instance = R_InstanceSizeFromCmdKind(cmd->kind);
   U64 floats_per_instance = bytes_per_instance / sizeof(F32);
   R_Handle albedo_texture_handle = R_HandleIsZero(cmd->albedo_texture) ? r_d3d11_state->backup_texture : cmd->albedo_texture;
   R_D3D11_Texture2D albedo_texture = R_D3D11_Texture2DFromHandle(albedo_texture_handle);
   Mat4x4F32 albedo_sample_channel_map = R_D3D11_SampleChannelMapFromTexture2DFormat(albedo_texture.format);
   R_Texture2DSampleKind sample_kind = cmd->albedo_texture_sample_kind;
   Rng2F32 clip = R2F32(V2F32(Clamp(0, Min(cmd->clip.x0, cmd->clip.x1), wnd->last_resolution.x),
                              Clamp(0, Min(cmd->clip.y0, cmd->clip.y1), wnd->last_resolution.y)),
                        V2F32(Clamp(0, Max(cmd->clip.x0, cmd->clip.x1), wnd->last_resolution.x),
                              Clamp(0, Max(cmd->clip.y0, cmd->clip.y1), wnd->last_resolution.y)));
   Mat4x4F32 view = cmd->view3d;
   Mat4x4F32 projection = cmd->projection3d;
   Mat4x4 view_projection = Mul4x4F32(projection, view);
   
   //- rjf: set up viewport
   if(viewport_change)
   {
    Vec2S64 resolution = wnd->last_resolution;
    D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)resolution.x, (F32)resolution.y, 0.0f, 1.0f };
    if(viewport.x0 != 0 || viewport.y0 != 0 || viewport.x1 != 0 || viewport.y1 != 0)
    {
     Vec2F32 viewport_dim = Dim2F32(viewport);
     d3d11_viewport.TopLeftX = viewport.x0;
     d3d11_viewport.TopLeftY = viewport.y0;
     d3d11_viewport.Width = viewport_dim.x;
     d3d11_viewport.Height = viewport_dim.y;
    }
    d_ctx->RSSetViewports(1, &d3d11_viewport);
    d_ctx->RSSetState(rasterizer);
   }
   
   //- rjf: send & grab instance count & buffer
   U64 instance_count = cmd->total_instance_count;
   ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromCmd(cmd);
   
   //- rjf: grab sampler
   ID3D11SamplerState *sampler = r_d3d11_state->linear_sampler;
   switch(sample_kind)
   {
    default:
    case R_Texture2DSampleKind_Nearest: {sampler = r_d3d11_state->nearest_sampler;}break;
    case R_Texture2DSampleKind_Linear:  {sampler = r_d3d11_state->linear_sampler;}break;
   }
   
   //- rjf: set up output merger
   if(flags_change)
   {
    if(new_flags & R_CmdFlag_DepthWrite && new_flags & R_CmdFlag_DepthTest)
    {
     d_ctx->OMSetDepthStencilState(r_d3d11_state->plain_depth_stencil_state, 1);
    }
    if(new_flags & R_CmdFlag_DepthWrite && !(new_flags & R_CmdFlag_DepthTest))
    {
     d_ctx->OMSetDepthStencilState(r_d3d11_state->write_depth_stencil_state, 1);
    }
    if(!(new_flags & R_CmdFlag_DepthWrite) && !(new_flags & R_CmdFlag_DepthTest))
    {
     d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 1);
    }
    d_ctx->OMSetRenderTargets(1, &framebuffer_view, depth_buffer_view);
    d_ctx->OMSetBlendState(r_d3d11_state->blend_state, 0, 0xffffffff);
   }
   
   //- rjf: setup scissor rect
   {
    D3D11_RECT rect =
    {
     /* .left   = */ 0,
     /* .top    = */ 0,
     /* .right  = */ (LONG)wnd->last_resolution.x,
     /* .bottom = */ (LONG)wnd->last_resolution.y,
    };
    if(clip.x0 != 0 || clip.y0 != 0 || clip.x1 != 0 || clip.y1 != 0)
    {
     rect.left = (LONG)clip.x0;
     rect.right = (LONG)clip.x1;
     rect.top = (LONG)clip.y0;
     rect.bottom = (LONG)clip.y1;
    }
    d_ctx->RSSetScissorRects(1, &rect);
   }
   
   //- rjf: set up depth writing
   
   
   //- rjf: process command
   switch(cmd->kind)
   {
    default:break;
    
    //- rjf: rects (2d)
    case R_CmdKind_Rect2D:
    {
     // rjf: grab pipeline objects
     ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_Rect2D];
     ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_Rect2D];
     ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_Rect2D];
     ID3D11InputLayout *ilay = r_d3d11_state->ilay_table[R_D3D11_ShaderPairKind_Rect2D];
     
     // rjf: send per-cmd globals
     R_D3D11_CmdGlobals_Rect2D cmd_globals = {0};
     {
      cmd_globals.viewport_size             = Vec2F32FromVec(wnd->last_resolution);
      cmd_globals.opacity                   = cmd->opacity;
      cmd_globals.albedo_sample_channel_map = albedo_sample_channel_map;
      cmd_globals.albedo_t2d_size           = Vec2F32FromVec(albedo_texture.size);
      cmd_globals.xform[0] = V4F32(cmd->xform2d.elements[0][0], cmd->xform2d.elements[1][0], cmd->xform2d.elements[2][0], 0);
      cmd_globals.xform[1] = V4F32(cmd->xform2d.elements[0][1], cmd->xform2d.elements[1][1], cmd->xform2d.elements[2][1], 0);
      cmd_globals.xform[2] = V4F32(cmd->xform2d.elements[0][2], cmd->xform2d.elements[1][2], cmd->xform2d.elements[2][2], 0);
      Vec2F32 xform_2x2_col0 = V2F32(cmd_globals.xform[0].x, cmd_globals.xform[1].x);
      Vec2F32 xform_2x2_col1 = V2F32(cmd_globals.xform[0].y, cmd_globals.xform[1].y);
      cmd_globals.xform_scale.x = Length2F32(xform_2x2_col0);
      cmd_globals.xform_scale.y = Length2F32(xform_2x2_col1);
     }
     R_D3D11_WriteGPUBuffer(d_ctx, cmd_global_buffer, 0, Str8Struct(&cmd_globals));
     
     // rjf: setup input assembly
     U32 stride = bytes_per_instance;
     U32 offset = 0;
     d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
     d_ctx->IASetInputLayout(ilay);
     d_ctx->IASetVertexBuffers(0, 1, &instance_buffer, &stride, &offset);
     
     // rjf: setup shaders
     d_ctx->VSSetShader(vshad, 0, 0);
     d_ctx->VSSetConstantBuffers(0, 1, &cmd_global_buffer);
     d_ctx->PSSetShader(pshad, 0, 0);
     d_ctx->PSSetConstantBuffers(0, 1, &cmd_global_buffer);
     d_ctx->PSSetShaderResources(0, 1, &albedo_texture.view);
     d_ctx->PSSetSamplers(0, 1, &sampler);
     
     // rjf: draw
     d_ctx->DrawInstanced(4, instance_count, 0, 0);
    }break;
    
    //- rjf: sprites (3d)
    case R_CmdKind_Sprite3D:
    {
     // rjf: grab pipeline objects
     ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_Sprite3D];
     ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_Sprite3D];
     ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_Sprite3D];
     ID3D11InputLayout *ilay = r_d3d11_state->ilay_table[R_D3D11_ShaderPairKind_Sprite3D];
     
     // rjf: send per-cmd globals
     R_D3D11_CmdGlobals_Sprite3D cmd_globals = {0};
     {
      cmd_globals.xform                     = view_projection;
      cmd_globals.albedo_sample_channel_map = albedo_sample_channel_map;
      cmd_globals.albedo_t2d_size           = Vec2F32FromVec(albedo_texture.size);
     }
     R_D3D11_WriteGPUBuffer(d_ctx, cmd_global_buffer, 0, Str8Struct(&cmd_globals));
     
     // rjf: setup input assembly
     U32 stride = bytes_per_instance;
     U32 offset = 0;
     d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
     d_ctx->IASetInputLayout(ilay);
     d_ctx->IASetVertexBuffers(0, 1, &instance_buffer, &stride, &offset);
     
     // rjf: setup shaders
     d_ctx->VSSetShader(vshad, 0, 0);
     d_ctx->VSSetConstantBuffers(0, 1, &cmd_global_buffer);
     d_ctx->PSSetShader(pshad, 0, 0);
     d_ctx->PSSetConstantBuffers(0, 1, &cmd_global_buffer);
     d_ctx->PSSetShaderResources(0, 1, &albedo_texture.view);
     d_ctx->PSSetSamplers(0, 1, &sampler);
     
     // rjf: draw
     d_ctx->DrawInstanced(4, instance_count, 0, 0);
    }break;
    
    //- rjf: debug lines (3d)
    case R_CmdKind_DebugLine3D:
    {
     // rjf: grab pipeline objects
     ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_DebugLine3D];
     ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_DebugLine3D];
     ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_DebugLine3D];
     ID3D11InputLayout *ilay = r_d3d11_state->ilay_table[R_D3D11_ShaderPairKind_DebugLine3D];
     
     // rjf: send per-cmd globals
     R_D3D11_CmdGlobals_DebugLine3D cmd_globals = {0};
     {
      cmd_globals.xform = view_projection;
     }
     R_D3D11_WriteGPUBuffer(d_ctx, cmd_global_buffer, 0, Str8Struct(&cmd_globals));
     
     // rjf: setup input assembly
     U32 stride = bytes_per_instance;
     U32 offset = 0;
     d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
     d_ctx->IASetInputLayout(ilay);
     d_ctx->IASetVertexBuffers(0, 1, &instance_buffer, &stride, &offset);
     
     // rjf: setup shaders
     d_ctx->VSSetShader(vshad, 0, 0);
     d_ctx->VSSetConstantBuffers(0, 1, &cmd_global_buffer);
     d_ctx->PSSetShader(pshad, 0, 0);
     d_ctx->PSSetConstantBuffers(0, 1, &cmd_global_buffer);
     
     // rjf: draw
     d_ctx->DrawInstanced(2, instance_count, 0, 0);
    }break;
    
   }
  }
 }
}

render_function void
R_WindowFinish(R_Handle window_eqp)
{
 OS_MutexBlock(r_d3d11_state->device_mutex)
 {
  R_D3D11_WindowEquip *wnd = R_D3D11_WindowEquipFromHandle(window_eqp);
  ID3D11DeviceContext1 *d_ctx = r_d3d11_state->device_ctx;
  IDXGISwapChain1 *swapchain = wnd->swapchain;
  ID3D11Texture2D *framebuffer = wnd->framebuffer;
  ID3D11RenderTargetView *framebuffer_view = wnd->framebuffer_view;
  swapchain->Present(1, 0);
  d_ctx->ClearState();
 }
}
