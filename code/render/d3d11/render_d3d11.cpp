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
 { "OTX",  0,          DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "SHR",  0,          DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "PAD",  0,       DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

global D3D11_INPUT_ELEMENT_DESC r_d3d11_g_pointlight3d_ilay_elements[] =
{
 { "VTX",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,                            0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
 { "POS",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,                            0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "COL",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
 { "PRM",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
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

r_function DXGI_FORMAT
R_D3D11_DXGIFormatFromTexture2DFormat(R_Tex2DFormat fmt)
{
 DXGI_FORMAT result = DXGI_FORMAT_R8G8B8A8_UNORM;
 switch(fmt)
 {
  case R_Tex2DFormat_R8:
  {
   result = DXGI_FORMAT_R8_UNORM;
  }break;
  
  default:
  case R_Tex2DFormat_RGBA8: {}break;
 }
 return result;
}

r_function R_D3D11_WindowEquip *
R_D3D11_WindowEquipFromHandle(R_Handle handle)
{
 R_D3D11_WindowEquip *result = (R_D3D11_WindowEquip *)handle.u64[0];
 return result;
}

r_function R_Handle
R_D3D11_HandleFromWindowEquip(R_D3D11_WindowEquip *equip)
{
 R_Handle h = {0};
 h.u64[0] = (U64)equip;
 return h;
}

r_function R_D3D11_Tex2D
R_D3D11_Tex2DFromHandle(R_Handle handle)
{
 R_D3D11_Tex2D texture = {0};
 texture.texture = (ID3D11Texture2D *)handle.u64[0];
 texture.view = (ID3D11ShaderResourceView *)handle.u64[1];
 texture.size.x = handle.u32[4];
 texture.size.y = handle.u32[5];
 texture.format = (R_Tex2DFormat)handle.u32[6];
 texture.kind = (R_Tex2DKind)handle.u32[7];
 return texture;
}

r_function R_Handle
R_D3D11_HandleFromTexture2D(R_D3D11_Tex2D texture)
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

r_function R_D3D11_Buffer
R_D3D11_BufferFromHandle(R_Handle handle)
{
 R_D3D11_Buffer buffer = {0};
 buffer.obj = (ID3D11Buffer *)handle.u64[0];
 buffer.size = handle.u64[1];
 return buffer;
}

r_function R_Handle
R_D3D11_HandleFromBuffer(R_D3D11_Buffer buffer)
{
 R_Handle handle = {0};
 handle.u64[0] = (U64)buffer.obj;
 handle.u64[1] = buffer.size;
 return handle;
}

r_function void
R_D3D11_BufferWriteString(ID3D11DeviceContext1 *device_ctx, ID3D11Buffer *buffer, String8 data)
{
 D3D11_MAPPED_SUBRESOURCE sub_rsrc = {0};
 device_ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub_rsrc);
 U8 *ptr = (U8 *)sub_rsrc.pData;
 MemoryCopy(ptr, data.str, data.size);
 device_ctx->Unmap(buffer, 0);
}

r_function ID3D11Buffer *
R_D3D11_InstanceBufferFromBatchList(R_BatchList *list)
{
 U64 needed_size = list->byte_count;
 
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
  D3D11_MAPPED_SUBRESOURCE sub_rsrc = {0};
  r_d3d11_state->device_ctx->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub_rsrc);
  U8 *ptr = (U8 *)sub_rsrc.pData;
  for(R_Batch *batch = list->first; batch != 0; batch = batch->next)
  {
   MemoryCopy(ptr, batch->v, batch->byte_count);
   ptr += batch->byte_count;
  }
  r_d3d11_state->device_ctx->Unmap(buffer, 0);
 }
 
 return buffer;
}

r_function Mat4x4F32
R_D3D11_SampleChannelMapFromTexture2DFormat(R_Tex2DFormat fmt)
{
 Mat4x4F32 map = MakeMat4x4F32(1.f);
 switch(fmt)
 {
  default: break;
  case R_Tex2DFormat_R8:
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

r_function R_InitReceipt
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
  D3D11_MESSAGE_ID hide[] =
  {
   D3D11_MESSAGE_ID_DEVICE_DRAW_RENDERTARGETVIEW_NOT_SET,
  };
  D3D11_INFO_QUEUE_FILTER filter = {0};
  filter.DenyList.NumIDs = ArrayCount(hide);
  filter.DenyList.pIDList = hide;
  info->AddStorageFilterEntries(&filter);
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
  
  //- rjf: make main blend state
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
   r_d3d11_state->device->CreateBlendState(&desc, &r_d3d11_state->main_blend_state);
  }
  
  //- rjf: make additive blend state
  {
   D3D11_BLEND_DESC desc = {};
   {
    desc.RenderTarget[0].BlendEnable            = 1;
    desc.RenderTarget[0].SrcBlend               = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend              = D3D11_BLEND_ONE; 
    desc.RenderTarget[0].BlendOp                = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha          = D3D11_BLEND_ZERO;
    desc.RenderTarget[0].DestBlendAlpha         = D3D11_BLEND_ONE;
    desc.RenderTarget[0].BlendOpAlpha           = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;
   }
   r_d3d11_state->device->CreateBlendState(&desc, &r_d3d11_state->additive_blend_state);
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
   {
    D3D11_DEPTH_STENCIL_DESC desc = {};
    {
     desc.DepthEnable    = TRUE;
     desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
     desc.DepthFunc      = D3D11_COMPARISON_LESS;
    }
    r_d3d11_state->device->CreateDepthStencilState(&desc, &r_d3d11_state->read_depth_stencil_state);
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
   if(info->element_descriptions != 0)
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
   r_d3d11_state->white_texture = R_Tex2DAlloc(V2S64(1, 1), R_Tex2DFormat_RGBA8, R_Tex2DKind_Static, &white_texture_data);
   
   // rjf: initialize backup texture
   U8 backup_texture_data[] =
   {
    0xff, 0x00, 0xff, 0xff,
    0x11, 0x00, 0x11, 0xff,
    0x11, 0x00, 0x11, 0xff,
    0xff, 0x00, 0xff, 0xff,
   };
   r_d3d11_state->backup_texture = R_Tex2DAlloc(V2S64(2, 2), R_Tex2DFormat_RGBA8, R_Tex2DKind_Static, &backup_texture_data[0]);
   
   // rjf: initialize light sphere vertex/index buffers
   {
    Temp scratch = ScratchBegin(0, 0);
    VertexIndexArrayPair vtx_idx_arrays = IcoSphereMake(scratch.arena, 0);
    {
     D3D11_SUBRESOURCE_DATA data = {0};
     {
      data.pSysMem = vtx_idx_arrays.vertices.v;
     }
     D3D11_BUFFER_DESC desc = {0};
     {
      desc.ByteWidth      = vtx_idx_arrays.vertices.count*sizeof(Vec3F32);
      desc.Usage          = D3D11_USAGE_IMMUTABLE;
      desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
     }
     r_d3d11_state->device->CreateBuffer(&desc, &data, &r_d3d11_state->light_sphere_vtx_buffer);
     r_d3d11_state->light_sphere_vtx_count = vtx_idx_arrays.vertices.count;
    }
    {
     D3D11_SUBRESOURCE_DATA data = {0};
     {
      data.pSysMem = vtx_idx_arrays.indices.v;
     }
     D3D11_BUFFER_DESC desc = {0};
     {
      desc.ByteWidth      = vtx_idx_arrays.indices.count*sizeof(S32);
      desc.Usage          = D3D11_USAGE_IMMUTABLE;
      desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
     }
     r_d3d11_state->device->CreateBuffer(&desc, &data, &r_d3d11_state->light_sphere_idx_buffer);
     r_d3d11_state->light_sphere_idx_count = vtx_idx_arrays.indices.count;
    }
    ScratchEnd(scratch);
   }
  }
  
  //- rjf: set up overflow state
  r_d3d11_state->overflow_arena = ArenaAlloc(Kilobytes(256));
 }
 R_InitReceipt result = {0};
 return result;
}

r_function R_Handle
R_WindowEquip(OS_Handle window_handle)
{
 R_D3D11_WindowEquip *eqp = (R_D3D11_WindowEquip *)OS_Reserve(sizeof(R_D3D11_WindowEquip));
 OS_Commit(eqp, sizeof(*eqp));
 OS_W32_Window *window = OS_W32_WindowFromHandle(window_handle);
 HWND hwnd = window->hwnd;
 OS_MutexScope(r_d3d11_state->device_mutex)
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
  
  //- rjf: get framebuffer & make rtv
  eqp->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)(&eqp->framebuffer));
  r_d3d11_state->device->CreateRenderTargetView(eqp->framebuffer, 0, &eqp->framebuffer_rtv);
  
  //- rjf: make shadow map resources
  {
   eqp->shadowmap_resolution = V2S64(1024, 1024);
   D3D11_TEXTURE2D_DESC depth_desc = {};
   {
    depth_desc.Width     = eqp->shadowmap_resolution.x;
    depth_desc.Height    = eqp->shadowmap_resolution.y;
    depth_desc.Format    = DXGI_FORMAT_R24G8_TYPELESS;
    depth_desc.ArraySize = 1;
    depth_desc.SampleDesc.Count = 1;
    depth_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_DEPTH_STENCIL;
   }
   D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
   {
    dsv_desc.Flags = 0;
    dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsv_desc.Texture2D.MipSlice = 0;
   }
   D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
   {
    srv_desc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srv_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MostDetailedMip = 0;
    srv_desc.Texture2D.MipLevels       = -1;
   }
   r_d3d11_state->device->CreateTexture2D(&depth_desc, 0, &eqp->shadowmap_depth);
   r_d3d11_state->device->CreateDepthStencilView(eqp->shadowmap_depth, &dsv_desc, &eqp->shadowmap_depth_dsv);
   r_d3d11_state->device->CreateShaderResourceView(eqp->shadowmap_depth, &srv_desc, &eqp->shadowmap_depth_srv);
  }
 }
 return R_D3D11_HandleFromWindowEquip(eqp);
}

r_function void
R_WindowUnequip(OS_Handle window, R_Handle window_eqp)
{
 R_D3D11_WindowEquip *eqp = R_D3D11_WindowEquipFromHandle(window_eqp);
 if(eqp->shadowmap_depth_srv)          { eqp->shadowmap_depth_srv->Release(); }
 if(eqp->shadowmap_depth_dsv)          { eqp->shadowmap_depth_dsv->Release(); }
 if(eqp->shadowmap_depth)              { eqp->shadowmap_depth->Release(); }
 if(eqp->gbuffer_depth_srv)            { eqp->gbuffer_depth_srv->Release(); }
 if(eqp->gbuffer_depth_dsv)            { eqp->gbuffer_depth_dsv->Release(); }
 if(eqp->gbuffer_depth)                { eqp->gbuffer_depth->Release(); }
 if(eqp->gbuffer_color_srv)            { eqp->gbuffer_color_srv->Release(); }
 if(eqp->gbuffer_color_rtv)            { eqp->gbuffer_color_rtv->Release(); }
 if(eqp->gbuffer_color)                { eqp->gbuffer_color->Release(); }
 if(eqp->gbuffer_scratch_color_srv)    { eqp->gbuffer_scratch_color_srv->Release(); }
 if(eqp->gbuffer_scratch_color_rtv)    { eqp->gbuffer_scratch_color_rtv->Release(); }
 if(eqp->gbuffer_scratch_color)        { eqp->gbuffer_scratch_color->Release(); }
 if(eqp->framebuffer_rtv)              { eqp->framebuffer_rtv->Release(); }
 if(eqp->framebuffer)                  { eqp->framebuffer->Release(); }
 if(eqp->swapchain)                    { eqp->swapchain->Release(); }
 OS_Release(eqp, sizeof(*eqp));
}

r_function R_Handle
R_Tex2DAlloc(Vec2S64 size, R_Tex2DFormat fmt, R_Tex2DKind kind, void *initial_data)
{
 // rjf: kind => usage
 D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
 switch(kind)
 {
  default:
  case R_Tex2DKind_Static: {usage = D3D11_USAGE_DYNAMIC;}break;
  case R_Tex2DKind_Dynamic:{usage = D3D11_USAGE_DEFAULT;}break;
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
  initial_subrsrc_data.SysMemPitch = R_BytesPerPixelFromTex2DFormat(fmt) * size.x;
  initial_subrsrc_data.SysMemSlicePitch = 0;
 }
 R_D3D11_Tex2D texture = {0};
 OS_MutexScope(r_d3d11_state->device_mutex)
 {
  r_d3d11_state->device->CreateTexture2D(&texture_desc, initial_data ? &initial_subrsrc_data : 0, &texture.texture);
  r_d3d11_state->device->CreateShaderResourceView(texture.texture, 0, &texture.view);
 }
 texture.size = Vec2S32FromVec(size);
 texture.format = fmt;
 return R_D3D11_HandleFromTexture2D(texture);
}

r_function void
R_Tex2DRelease(R_Handle texture_handle)
{
 OS_MutexScope(r_d3d11_state->device_mutex)
 {
  R_D3D11_Tex2D texture = R_D3D11_Tex2DFromHandle(texture_handle);
  texture.texture->Release();
  texture.view->Release();
 }
}

r_function void
R_Tex2DFillRegion(R_Handle texture_handle, Rng2S64 region, void *data)
{
 OS_MutexScope(r_d3d11_state->device_mutex)
 {
  R_D3D11_Tex2D tex = R_D3D11_Tex2DFromHandle(texture_handle);
  U64 bytes_per_pixel = R_BytesPerPixelFromTex2DFormat(tex.format);
  Vec2S64 dim = Dim2S64(region);
  D3D11_BOX dst_box =
  {
   (UINT)region.x0, (UINT)region.y0, 0,
   (UINT)region.x1, (UINT)region.y1, 1,
  };
  r_d3d11_state->device_ctx->UpdateSubresource(tex.texture, 0, &dst_box, data, dim.x*bytes_per_pixel, 0);
 }
}

r_function Vec2S64
R_SizeFromTex2D(R_Handle texture_handle)
{
 R_D3D11_Tex2D tex = R_D3D11_Tex2DFromHandle(texture_handle);
 return Vec2S64FromVec(tex.size);
}

r_function R_Tex2DFormat
R_FormatFromTex2D(R_Handle texture_handle)
{
 R_D3D11_Tex2D tex = R_D3D11_Tex2DFromHandle(texture_handle);
 return tex.format;
}

r_function void
R_BeginFrame(void)
{
 // NOTE(rjf): no-op
}

r_function void
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

r_function void
R_WindowStart(R_Handle window_eqp, Vec2S64 resolution)
{
 OS_MutexScope(r_d3d11_state->device_mutex)
 {
  R_D3D11_WindowEquip *wnd = R_D3D11_WindowEquipFromHandle(window_eqp);
  ID3D11Device1 *device = r_d3d11_state->device;
  ID3D11DeviceContext1 *d_ctx = r_d3d11_state->device_ctx;
  
  //- rjf: resolution changed
  B32 resolution_changed = (wnd->last_resolution.x != resolution.x ||
                            wnd->last_resolution.y != resolution.y);
  wnd->last_resolution = resolution;
  
  //- rjf: set up swapchain
  if(resolution_changed)
  {
   wnd->framebuffer_rtv->Release();
   wnd->framebuffer->Release();
   wnd->swapchain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
   wnd->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)(&wnd->framebuffer));
   r_d3d11_state->device->CreateRenderTargetView(wnd->framebuffer, 0, &wnd->framebuffer_rtv);
  }
  
  //- rjf: set up g-buffer
  if(resolution_changed)
  {
   // rjf: release old resources, if there
   if(wnd->gbuffer_depth_srv)        {wnd->gbuffer_depth_srv->Release();}
   if(wnd->gbuffer_depth_dsv)        {wnd->gbuffer_depth_dsv->Release();}
   if(wnd->gbuffer_depth)            {wnd->gbuffer_depth->Release();}
   if(wnd->gbuffer_color_srv)        {wnd->gbuffer_color_srv->Release();}
   if(wnd->gbuffer_color_rtv)        {wnd->gbuffer_color_rtv->Release();}
   if(wnd->gbuffer_color)            {wnd->gbuffer_color->Release();}
   if(wnd->gbuffer_scratch_color_srv){wnd->gbuffer_scratch_color_srv->Release();}
   if(wnd->gbuffer_scratch_color_rtv){wnd->gbuffer_scratch_color_rtv->Release();}
   if(wnd->gbuffer_scratch_color)    {wnd->gbuffer_scratch_color->Release();}
   
   // rjf: create g-buffer color target
   {
    D3D11_TEXTURE2D_DESC color_desc = {};
    {
     wnd->framebuffer->GetDesc(&color_desc);
     color_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
     color_desc.BindFlags = D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
    }
    D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
    {
     rtv_desc.Format         = color_desc.Format;
     rtv_desc.ViewDimension  = D3D11_RTV_DIMENSION_TEXTURE2D;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    {
     srv_desc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
     srv_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
     srv_desc.Texture2D.MipLevels       = -1;
    }
    device->CreateTexture2D(&color_desc, 0, &wnd->gbuffer_color);
    device->CreateRenderTargetView(wnd->gbuffer_color, &rtv_desc, &wnd->gbuffer_color_rtv);
    device->CreateShaderResourceView(wnd->gbuffer_color, &srv_desc, &wnd->gbuffer_color_srv);
   }
   
   // rjf: create g-buffer depth target
   {
    D3D11_TEXTURE2D_DESC depth_desc = {};
    {
     wnd->framebuffer->GetDesc(&depth_desc);
     depth_desc.Format    = DXGI_FORMAT_R24G8_TYPELESS;
     depth_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE|D3D11_BIND_DEPTH_STENCIL;
    }
    D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
    {
     dsv_desc.Flags = 0;
     dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
     dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
     dsv_desc.Texture2D.MipSlice = 0;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    {
     srv_desc.Format                    = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
     srv_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
     srv_desc.Texture2D.MostDetailedMip = 0;
     srv_desc.Texture2D.MipLevels       = -1;
    }
    device->CreateTexture2D(&depth_desc, 0, &wnd->gbuffer_depth);
    device->CreateDepthStencilView(wnd->gbuffer_depth, &dsv_desc, &wnd->gbuffer_depth_dsv);
    device->CreateShaderResourceView(wnd->gbuffer_depth, &srv_desc, &wnd->gbuffer_depth_srv);
   }
   
   // rjf: create g-buffer scratch color target
   {
    D3D11_TEXTURE2D_DESC color_desc = {};
    {
     wnd->framebuffer->GetDesc(&color_desc);
     color_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
     color_desc.BindFlags = D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE;
    }
    D3D11_RENDER_TARGET_VIEW_DESC rtv_desc = {};
    {
     rtv_desc.Format         = color_desc.Format;
     rtv_desc.ViewDimension  = D3D11_RTV_DIMENSION_TEXTURE2D;
    }
    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc = {};
    {
     srv_desc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;
     srv_desc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
     srv_desc.Texture2D.MipLevels       = -1;
    }
    device->CreateTexture2D(&color_desc, 0, &wnd->gbuffer_scratch_color);
    device->CreateRenderTargetView(wnd->gbuffer_scratch_color, &rtv_desc, &wnd->gbuffer_scratch_color_rtv);
    device->CreateShaderResourceView(wnd->gbuffer_scratch_color, &srv_desc, &wnd->gbuffer_scratch_color_srv);
   }
   
  }
  
  //- rjf: clear
  Vec4F32 clear_color = {0};
  d_ctx->ClearRenderTargetView(wnd->framebuffer_rtv, clear_color.v);
 }
}

r_function void
R_WindowSubmit(R_Handle window_eqp, R_PassList *passes)
{
 OS_MutexScope(r_d3d11_state->device_mutex)
 {
  R_D3D11_WindowEquip *wnd = R_D3D11_WindowEquipFromHandle(window_eqp);
  ID3D11DeviceContext1 *d_ctx = r_d3d11_state->device_ctx;
  
  //- rjf: do all of the passes
  for(R_PassNode *pass_node = passes->first; pass_node != 0; pass_node = pass_node->next)
  {
   R_Pass *pass = &pass_node->v;
   switch(pass->kind)
   {
    default:
    {
     // NOTE(rjf): this pass is not supported
    }break;
    
    //- rjf: UI pass
    case R_PassKind_UI:
    {
     R_PassParams_UI *params = pass->params_ui;
     
     //- rjf: draw rectangle batches
     for(R_BatchGroup2DNode *group_n = params->rects.first; group_n != 0; group_n = group_n->next)
     {
      // rjf: unpack
      R_BatchList *batches = &group_n->batches;
      R_BatchGroup2DParams *batch_params = &group_n->params;
      ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromBatchList(batches);
      U64 instance_count = batches->inst_count;
      U64 bytes_per_instance = batches->byte_count/batches->inst_count;
      R_Handle albedo_texture_handle = R_HandleIsZero(batch_params->albedo_tex) ? r_d3d11_state->backup_texture : batch_params->albedo_tex;
      R_D3D11_Tex2D albedo_texture = R_D3D11_Tex2DFromHandle(albedo_texture_handle);
      Mat4x4F32 albedo_sample_channel_map = R_D3D11_SampleChannelMapFromTexture2DFormat(albedo_texture.format);
      R_Tex2DSampleKind sample_kind = batch_params->albedo_tex_sample_kind;
      ID3D11SamplerState *sampler = r_d3d11_state->linear_sampler;
      switch(sample_kind)
      {
       default:
       case R_Tex2DSampleKind_Nearest: {sampler = r_d3d11_state->nearest_sampler;}break;
       case R_Tex2DSampleKind_Linear:  {sampler = r_d3d11_state->linear_sampler;}break;
      }
      Rng2F32 clip = R2F32(V2F32(Clamp(0, Min(batch_params->clip.x0, batch_params->clip.x1), wnd->last_resolution.x),
                                 Clamp(0, Min(batch_params->clip.y0, batch_params->clip.y1), wnd->last_resolution.y)),
                           V2F32(Clamp(0, Max(batch_params->clip.x0, batch_params->clip.x1), wnd->last_resolution.x),
                                 Clamp(0, Max(batch_params->clip.y0, batch_params->clip.y1), wnd->last_resolution.y)));
      
      // rjf: set up viewport & rasterizer
      {
       Vec2S64 resolution = wnd->last_resolution;
       D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)resolution.x, (F32)resolution.y, 0.0f, 1.0f };
       if(params->viewport.x0 != 0 || params->viewport.x1 != 0 ||
          params->viewport.y0 != 0 || params->viewport.y1 != 0)
       {
        Vec2F32 dim = Dim2F32(params->viewport);
        d3d11_viewport.TopLeftX = params->viewport.x0;
        d3d11_viewport.TopLeftY = params->viewport.y0;
        d3d11_viewport.Width = dim.x;
        d3d11_viewport.Height = dim.y;
       }
       d_ctx->RSSetViewports(1, &d3d11_viewport);
       d_ctx->RSSetState(r_d3d11_state->rasterizer);
      }
      
      // rjf: setup scissor rect
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
      
      // rjf: setup output merger
      {
       d_ctx->OMSetRenderTargets(1, &wnd->framebuffer_rtv, 0);
       d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
       d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 0);
      }
      
      // rjf: grab pipeline objects
      ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_Rect2D];
      ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_Rect2D];
      ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_Rect2D];
      ID3D11InputLayout *ilay = r_d3d11_state->ilay_table[R_D3D11_ShaderPairKind_Rect2D];
      
      // rjf: send per-cmd globals
      R_D3D11_CmdGlobals_Rect2D cmd_globals = {0};
      {
       cmd_globals.viewport_size             = Vec2F32FromVec(wnd->last_resolution);
       cmd_globals.opacity                   = 1-batch_params->transparency;
       cmd_globals.albedo_sample_channel_map = albedo_sample_channel_map;
       cmd_globals.albedo_t2d_size           = Vec2F32FromVec(albedo_texture.size);
       cmd_globals.xform[0] = V4F32(batch_params->xform2d.elements[0][0], batch_params->xform2d.elements[1][0], batch_params->xform2d.elements[2][0], 0);
       cmd_globals.xform[1] = V4F32(batch_params->xform2d.elements[0][1], batch_params->xform2d.elements[1][1], batch_params->xform2d.elements[2][1], 0);
       cmd_globals.xform[2] = V4F32(batch_params->xform2d.elements[0][2], batch_params->xform2d.elements[1][2], batch_params->xform2d.elements[2][2], 0);
       Vec2F32 xform_2x2_col0 = V2F32(cmd_globals.xform[0].x, cmd_globals.xform[1].x);
       Vec2F32 xform_2x2_col1 = V2F32(cmd_globals.xform[0].y, cmd_globals.xform[1].y);
       cmd_globals.xform_scale.x = Length2F32(xform_2x2_col0);
       cmd_globals.xform_scale.y = Length2F32(xform_2x2_col1);
      }
      R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
      
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
     }
    }break;
    
    //- rjf: G0 pass
    case R_PassKind_G0:
    {
     R_PassParams_G0 *params = pass->params_g0;
     Mat4x4F32 view_projection = Mul4x4F32(params->xform_projection, params->xform_view);
     Mat4x4F32 inverse_view_projection = Inverse4x4F32(view_projection);
     Vec2S64 resolution = wnd->last_resolution;
     D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)resolution.x, (F32)resolution.y, 0.0f, 1.0f };
     if(params->viewport.x0 != 0 || params->viewport.x1 != 0 ||
        params->viewport.y0 != 0 || params->viewport.y1 != 0)
     {
      Vec2F32 dim = Dim2F32(params->viewport);
      d3d11_viewport.TopLeftX = params->viewport.x0;
      d3d11_viewport.TopLeftY = params->viewport.y0;
      d3d11_viewport.Width = dim.x;
      d3d11_viewport.Height = dim.y;
     }
     Vec3F32 keylight_eye = Scale3F32(Normalize3F32(params->keylight_dir), -1.f*20.f);
     Mat4x4F32 shadowmap_view = MakeLookAt4x4F32(keylight_eye, V3F32(0, 0, 0), V3F32(0, 0, 1));
     Mat4x4F32 shadowmap_proj = MakeOrthographic4x4F32(-30.f, +30.f, -30.f, +30.f, -100.f, 100.f);
     Mat4x4F32 shadowmap_view_proj = Mul4x4F32(shadowmap_proj, shadowmap_view);
     
     //- rjf: clear gbuffer & shadowmap
     {
      Vec4F32 clear_color = {0};
      d_ctx->ClearRenderTargetView(wnd->gbuffer_color_rtv, clear_color.v);
      d_ctx->ClearDepthStencilView(wnd->gbuffer_depth_dsv, D3D11_CLEAR_DEPTH, 1.f, 0);
      d_ctx->ClearDepthStencilView(wnd->shadowmap_depth_dsv, D3D11_CLEAR_DEPTH, 1.f, 0);
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
      if(params->viewport.x0 != 0 || params->viewport.x1 != 0 ||
         params->viewport.y0 != 0 || params->viewport.y1 != 0)
      {
       rect.left   = params->viewport.x0;
       rect.top    = params->viewport.y0;
       rect.right  = params->viewport.x1;
       rect.bottom = params->viewport.y1;
      }
      d_ctx->RSSetScissorRects(1, &rect);
     }
     
     //- rjf: set up rasterizer
     {
      d_ctx->RSSetState(r_d3d11_state->rasterizer);
     }
     
     //- rjf: gbuffer <= opaque sprites
     {
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->plain_depth_stencil_state, 0);
      d_ctx->OMSetRenderTargets(1, &wnd->gbuffer_color_rtv, wnd->gbuffer_depth_dsv);
      for(U64 slot_idx = 0; slot_idx < params->sprites.slots_count; slot_idx += 1)
      {
       R_BatchGroup3DSlot *slot = &params->sprites.slots[slot_idx];
       for(R_BatchGroup3DNode *node = slot->first; node != 0; node = node->next)
       {
        R_BatchList *batches = &node->batches;
        R_BatchGroup3DParams *params = &node->params;
        
        // rjf: skip non-opaque
        if(params->blend_mode != R_BlendMode_Normal)
        {
         continue;
        }
        
        // rjf: unpack command batch data
        U64 bytes_per_instance = batches->byte_count/batches->inst_count;
        ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromBatchList(batches);
        R_Handle albedo_tex_handle = R_HandleIsZero(params->albedo_tex) ? r_d3d11_state->backup_texture : params->albedo_tex;
        R_D3D11_Tex2D albedo_tex = R_D3D11_Tex2DFromHandle(albedo_tex_handle);
        Mat4x4F32 albedo_sample_channel_map = R_D3D11_SampleChannelMapFromTexture2DFormat(albedo_tex.format);
        R_Tex2DSampleKind sample_kind = params->albedo_tex_sample_kind;
        ID3D11SamplerState *sampler = r_d3d11_state->linear_sampler;
        switch(sample_kind)
        {
         default:
         case R_Tex2DSampleKind_Nearest: {sampler = r_d3d11_state->nearest_sampler;}break;
         case R_Tex2DSampleKind_Linear:  {sampler = r_d3d11_state->linear_sampler;}break;
        }
        
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
         cmd_globals.albedo_t2d_size           = Vec2F32FromVec(albedo_tex.size);
         cmd_globals.alpha_test_min            = 1.f;
        }
        R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
        
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
        d_ctx->PSSetShaderResources(0, 1, &albedo_tex.view);
        d_ctx->PSSetSamplers(0, 1, &sampler);
        
        // rjf: draw
        d_ctx->DrawInstanced(4, batches->inst_count, 0, 0);
       }
      }
     }
     
     //- rjf: shadowmap <= opaque sprites
     {
      D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)wnd->shadowmap_resolution.x, (F32)wnd->shadowmap_resolution.y, 0.0f, 1.0f };
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->OMSetRenderTargets(0, 0, wnd->shadowmap_depth_dsv);
      d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->plain_depth_stencil_state, 0);
      for(U64 slot_idx = 0; slot_idx < params->sprites.slots_count; slot_idx += 1)
      {
       R_BatchGroup3DSlot *slot = &params->sprites.slots[slot_idx];
       for(R_BatchGroup3DNode *node = slot->first; node != 0; node = node->next)
       {
        R_BatchList *batches = &node->batches;
        R_BatchGroup3DParams *params = &node->params;
        
        // rjf: additive blending -> skip
        if(params->blend_mode != R_BlendMode_Normal)
        {
         continue;
        }
        
        // rjf: unpack command batch data
        U64 bytes_per_instance = batches->byte_count/batches->inst_count;
        ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromBatchList(batches);
        R_Handle albedo_tex_handle = R_HandleIsZero(params->albedo_tex) ? r_d3d11_state->backup_texture : params->albedo_tex;
        R_D3D11_Tex2D albedo_tex = R_D3D11_Tex2DFromHandle(albedo_tex_handle);
        Mat4x4F32 albedo_sample_channel_map = R_D3D11_SampleChannelMapFromTexture2DFormat(albedo_tex.format);
        R_Tex2DSampleKind sample_kind = params->albedo_tex_sample_kind;
        ID3D11SamplerState *sampler = r_d3d11_state->linear_sampler;
        switch(sample_kind)
        {
         default:
         case R_Tex2DSampleKind_Nearest: {sampler = r_d3d11_state->nearest_sampler;}break;
         case R_Tex2DSampleKind_Linear:  {sampler = r_d3d11_state->linear_sampler;}break;
        }
        
        // rjf: grab pipeline objects
        ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_Sprite3D];
        ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_Sprite3D];
        ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_Sprite3D];
        ID3D11InputLayout *ilay = r_d3d11_state->ilay_table[R_D3D11_ShaderPairKind_Sprite3D];
        
        // rjf: send per-cmd globals
        R_D3D11_CmdGlobals_Sprite3D cmd_globals = {0};
        {
         cmd_globals.xform                     = shadowmap_view_proj;
         cmd_globals.albedo_sample_channel_map = albedo_sample_channel_map;
         cmd_globals.albedo_t2d_size           = Vec2F32FromVec(albedo_tex.size);
         cmd_globals.alpha_test_min            = 1.f;
        }
        R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
        
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
        d_ctx->PSSetShaderResources(0, 1, &albedo_tex.view);
        d_ctx->PSSetSamplers(0, 1, &sampler);
        
        // rjf: draw
        d_ctx->DrawInstanced(4, batches->inst_count, 0, 0);
       }
      }
     }
     
     //- rjf: gbuffer-scratch <=unlit-composite= gbuffer
     {
      D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)resolution.x, (F32)resolution.y, 0.0f, 1.0f };
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->OMSetRenderTargets(1, &wnd->gbuffer_scratch_color_rtv, 0);
      d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 0);
      
      // rjf: grab pipeline objects
      ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_CompositeUnlit3D];
      ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_CompositeUnlit3D];
      ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_CompositeUnlit3D];
      
      // rjf: send per-cmd globals
      R_D3D11_CmdGlobals_CompositeUnlit3D cmd_globals = {0};
      {
       cmd_globals.inverse_view_projection   = inverse_view_projection;
       cmd_globals.shadowmap_view_projection = shadowmap_view_proj;
       cmd_globals.fog_color                 = params->fog_color;
       cmd_globals.pct_fog_per_unit          = params->pct_fog_per_unit;
       cmd_globals.near_z                    = params->near_z;
       cmd_globals.far_z                     = params->far_z;
      }
      R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
      
      // rjf: setup input assembly
      d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
      d_ctx->IASetInputLayout(0);
      
      // rjf: setup shaders
      d_ctx->VSSetShader(vshad, 0, 0);
      d_ctx->VSSetConstantBuffers(0, 1, &cmd_global_buffer);
      d_ctx->PSSetShader(pshad, 0, 0);
      d_ctx->PSSetConstantBuffers(0, 1, &cmd_global_buffer);
      d_ctx->PSSetShaderResources(0, 1, &wnd->gbuffer_color_srv);
      d_ctx->PSSetSamplers(0, 1, &r_d3d11_state->linear_sampler);
      d_ctx->PSSetShaderResources(1, 1, &wnd->gbuffer_depth_srv);
      d_ctx->PSSetSamplers(1, 1, &r_d3d11_state->linear_sampler);
      d_ctx->PSSetShaderResources(2, 1, &wnd->shadowmap_depth_srv);
      d_ctx->PSSetSamplers(2, 1, &r_d3d11_state->linear_sampler);
      
      // rjf: draw
      d_ctx->Draw(4, 0);
      
      // rjf: unset
      ID3D11ShaderResourceView *srv = 0;
      d_ctx->PSSetShaderResources(0, 1, &srv);
      d_ctx->PSSetShaderResources(1, 1, &srv);
      d_ctx->PSSetShaderResources(2, 1, &srv);
     }
     
     //- rjf: gbuffer <=blit= gbuffer-scratch
     {
      D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)resolution.x, (F32)resolution.y, 0.0f, 1.0f };
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->PSSetShaderResources(0, 0, 0);
      d_ctx->OMSetRenderTargets(1, &wnd->gbuffer_color_rtv, 0);
      d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 0);
      
      // rjf: grab pipeline objects
      ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_FramebufferBlit];
      ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_FramebufferBlit];
      
      // rjf: setup input assembly
      d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
      d_ctx->IASetInputLayout(0);
      
      // rjf: setup shaders
      d_ctx->VSSetShader(vshad, 0, 0);
      d_ctx->PSSetShader(pshad, 0, 0);
      d_ctx->PSSetShaderResources(0, 1, &wnd->gbuffer_scratch_color_srv);
      d_ctx->PSSetSamplers(0, 1, &r_d3d11_state->linear_sampler);
      
      // rjf: draw
      d_ctx->Draw(4, 0);
     }
     
     //- rjf: gbuffer <= light volumes
     if(params->point_lights.inst_count != 0)
     {
      R_BatchList *batch_list = &params->point_lights;
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->OMSetRenderTargets(1, &wnd->gbuffer_color_rtv, 0);
      d_ctx->OMSetBlendState(r_d3d11_state->additive_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 0);
      
      // rjf: unpack command batch data
      U64 index_count = r_d3d11_state->light_sphere_idx_count;
      U64 bytes_per_instance = batch_list->byte_count/batch_list->inst_count;
      U64 instance_count = batch_list->inst_count;
      ID3D11Buffer *vertex_buffer = r_d3d11_state->light_sphere_vtx_buffer;
      ID3D11Buffer *index_buffer = r_d3d11_state->light_sphere_idx_buffer;
      ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromBatchList(batch_list);
      
      // rjf: grab pipeline objects
      ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_PointLight3D];
      ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_PointLight3D];
      ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_PointLight3D];
      ID3D11InputLayout *ilay = r_d3d11_state->ilay_table[R_D3D11_ShaderPairKind_PointLight3D];
      
      // rjf: send per-cmd globals
      R_D3D11_CmdGlobals_PointLight3D cmd_globals = {0};
      {
       cmd_globals.xform = view_projection;
       cmd_globals.inverse_view_projection = inverse_view_projection;
      }
      R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
      
      // rjf: setup input assembly
      U32 ia_strides[] = {(U32)sizeof(Vec3F32), (U32)bytes_per_instance};
      U32 ia_offsets[] = {0, 0};
      ID3D11Buffer *ia_buffers[] = {vertex_buffer, instance_buffer};
      d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
      d_ctx->IASetInputLayout(ilay);
      d_ctx->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
      d_ctx->IASetVertexBuffers(0, ArrayCount(ia_buffers), ia_buffers, ia_strides, ia_offsets);
      
      // rjf: setup shaders
      d_ctx->VSSetShader(vshad, 0, 0);
      d_ctx->VSSetConstantBuffers(0, 1, &cmd_global_buffer);
      d_ctx->PSSetShader(pshad, 0, 0);
      d_ctx->PSSetConstantBuffers(0, 1, &cmd_global_buffer);
      d_ctx->PSSetShaderResources(0, 1, &wnd->gbuffer_depth_srv);
      d_ctx->PSSetSamplers(0, 1, &r_d3d11_state->linear_sampler);
      
      // rjf: draw
      d_ctx->DrawIndexedInstanced(index_count, instance_count, 0, 0, 0);
      
      // rjf: unset
      ID3D11ShaderResourceView *srv = 0;
      d_ctx->PSSetShaderResources(0, 1, &srv);
     }
     
     //- rjf: gbuffer <= emissive sprites
     {
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->OMSetBlendState(r_d3d11_state->additive_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->read_depth_stencil_state, 0);
      d_ctx->OMSetRenderTargets(1, &wnd->gbuffer_color_rtv, wnd->gbuffer_depth_dsv);
      for(U64 slot_idx = 0; slot_idx < params->sprites.slots_count; slot_idx += 1)
      {
       R_BatchGroup3DSlot *slot = &params->sprites.slots[slot_idx];
       for(R_BatchGroup3DNode *node = slot->first; node != 0; node = node->next)
       {
        R_BatchList *batches = &node->batches;
        R_BatchGroup3DParams *params = &node->params;
        
        // rjf: skip non-emissive
        if(params->blend_mode != R_BlendMode_Additive)
        {
         continue;
        }
        
        // rjf: unpack command batch data
        U64 bytes_per_instance = batches->byte_count/batches->inst_count;
        ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromBatchList(batches);
        R_Handle albedo_tex_handle = R_HandleIsZero(params->albedo_tex) ? r_d3d11_state->backup_texture : params->albedo_tex;
        R_D3D11_Tex2D albedo_tex = R_D3D11_Tex2DFromHandle(albedo_tex_handle);
        Mat4x4F32 albedo_sample_channel_map = R_D3D11_SampleChannelMapFromTexture2DFormat(albedo_tex.format);
        R_Tex2DSampleKind sample_kind = params->albedo_tex_sample_kind;
        ID3D11SamplerState *sampler = r_d3d11_state->linear_sampler;
        switch(sample_kind)
        {
         default:
         case R_Tex2DSampleKind_Nearest: {sampler = r_d3d11_state->nearest_sampler;}break;
         case R_Tex2DSampleKind_Linear:  {sampler = r_d3d11_state->linear_sampler;}break;
        }
        
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
         cmd_globals.albedo_t2d_size           = Vec2F32FromVec(albedo_tex.size);
         cmd_globals.alpha_test_min            = 0.f;
        }
        R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
        
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
        d_ctx->PSSetShaderResources(0, 1, &albedo_tex.view);
        d_ctx->PSSetSamplers(0, 1, &sampler);
        
        // rjf: draw
        d_ctx->DrawInstanced(4, batches->inst_count, 0, 0);
       }
      }
     }
     
     //- rjf: framebuffer <=lit-composite= gbuffer
     {
      D3D11_VIEWPORT d3d11_viewport = { 0.0f, 0.0f, (F32)resolution.x, (F32)resolution.y, 0.0f, 1.0f };
      d_ctx->RSSetViewports(1, &d3d11_viewport);
      d_ctx->OMSetRenderTargets(1, &wnd->framebuffer_rtv, 0);
      d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 0);
      
      // rjf: grab pipeline objects
      ID3D11Buffer *cmd_global_buffer = r_d3d11_state->cmd_global_buffer_table[R_D3D11_CmdGlobalKind_CompositeLit3D];
      ID3D11VertexShader *vshad = r_d3d11_state->vshad_table[R_D3D11_ShaderPairKind_CompositeLit3D];
      ID3D11PixelShader *pshad = r_d3d11_state->pshad_table[R_D3D11_ShaderPairKind_CompositeLit3D];
      
      // rjf: send per-cmd globals
      R_D3D11_CmdGlobals_CompositeLit3D cmd_globals = {0};
      {
       cmd_globals.inverse_view_projection   = inverse_view_projection;
       cmd_globals.fog_color                 = params->fog_color;
       cmd_globals.pct_fog_per_unit          = params->pct_fog_per_unit;
       cmd_globals.near_z                    = params->near_z;
       cmd_globals.far_z                     = params->far_z;
      }
      R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
      
      // rjf: setup input assembly
      d_ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
      d_ctx->IASetInputLayout(0);
      
      // rjf: setup shaders
      d_ctx->VSSetShader(vshad, 0, 0);
      d_ctx->VSSetConstantBuffers(0, 1, &cmd_global_buffer);
      d_ctx->PSSetShader(pshad, 0, 0);
      d_ctx->PSSetConstantBuffers(0, 1, &cmd_global_buffer);
      d_ctx->PSSetShaderResources(0, 1, &wnd->gbuffer_color_srv);
      d_ctx->PSSetSamplers(0, 1, &r_d3d11_state->linear_sampler);
      d_ctx->PSSetShaderResources(1, 1, &wnd->gbuffer_depth_srv);
      d_ctx->PSSetSamplers(1, 1, &r_d3d11_state->linear_sampler);
      
      // rjf: draw
      d_ctx->Draw(4, 0);
     }
     
     //- rjf: framebuffer <= debug lines
     if(params->debug_lines.inst_count != 0)
     {
      R_BatchList *batch_list = &params->debug_lines;
      d_ctx->OMSetRenderTargets(1, &wnd->framebuffer_rtv, 0);
      d_ctx->OMSetBlendState(r_d3d11_state->main_blend_state, 0, 0xffffffff);
      d_ctx->OMSetDepthStencilState(r_d3d11_state->noop_depth_stencil_state, 0);
      
      // rjf: unpack command batch data
      U64 bytes_per_instance = batch_list->byte_count/batch_list->inst_count;
      U64 instance_count = batch_list->inst_count;
      ID3D11Buffer *instance_buffer = R_D3D11_InstanceBufferFromBatchList(batch_list);
      
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
      R_D3D11_BufferWriteString(d_ctx, cmd_global_buffer, Str8Struct(&cmd_globals));
      
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
     }
    }break;
   }
  }
 }
}

r_function void
R_WindowFinish(R_Handle window_eqp)
{
 OS_MutexScope(r_d3d11_state->device_mutex)
 {
  R_D3D11_WindowEquip *wnd = R_D3D11_WindowEquipFromHandle(window_eqp);
  ID3D11DeviceContext1 *d_ctx = r_d3d11_state->device_ctx;
  wnd->swapchain->Present(1, 0);
  d_ctx->ClearState();
 }
}
