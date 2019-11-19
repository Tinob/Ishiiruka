// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Common/BitSet.h"
#include "Common/CommonTypes.h"
#include "Common/FileUtil.h"
#include "Common/LinearDiskCache.h"
#include "Common/MsgHandler.h"
#include "Common/StringUtil.h"
#include "Common/Logging/Log.h"

#include "Core/ConfigManager.h"

#include "VideoBackends/D3D12/D3DBase.h"
#include "VideoBackends/D3D12/D3DState.h"
#include "VideoBackends/D3D12/D3DUtil.h"
#include "VideoBackends/D3D12/D3DDescriptorHeapManager.h"
#include "VideoBackends/D3D12/NativeVertexFormat.h"
#include "VideoBackends/D3D12/ShaderCache.h"
#include "VideoBackends/D3D12/StaticShaderCache.h"

#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VideoConfig.h"
#include "VideoCommon/SamplerCommon.h"

namespace DX12
{

static bool s_cache_is_corrupted = false;
LinearDiskCache<SmallPsoDiskDesc, u8> s_pso_disk_cache;

class PipelineStateCacheInserter : public LinearDiskCacheReader<SmallPsoDiskDesc, u8>
{
public:
  void Read(const SmallPsoDiskDesc &key, const u8* value, u32 value_size)
  {
    if (s_cache_is_corrupted)
      return;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
    desc.GS = ShaderCache::GetGeometryShaderFromUid(key.gs_uid);
    if (key.using_uber_pixel_shader)
    {
      desc.PS = ShaderCache::GetPixelUberShaderFromUid(key.pus_uid);
    }
    else
    {
      desc.PS = ShaderCache::GetPixelShaderFromUid(key.ps_uid);
    }
    if (key.using_uber_vertex_shader)
    {
      desc.VS = ShaderCache::GetVertexShaderFromUid(key.vs_uid);
    }
    else
    {
      desc.VS = ShaderCache::GetVertexUberShaderFromUid(key.vus_uid);
    }
    desc.HS = ShaderCache::GetHullShaderFromUid(key.hds_uid);
    desc.DS = ShaderCache::GetDomainShaderFromUid(key.hds_uid);
    D3D::SetRootSignature(desc.GS.pShaderBytecode != nullptr, desc.HS.pShaderBytecode != nullptr, false);
    desc.pRootSignature = D3D::GetRootSignature(static_cast<size_t>(key.root_signature_index));
    desc.RTVFormats[0] = key.rtformat; // This state changes in PSTextureEncoder::Encode.
    desc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // This state changes in PSTextureEncoder::Encode.
    desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF;
    desc.NumRenderTargets = 1;
    desc.SampleMask = UINT_MAX;
    desc.SampleDesc = key.sample_desc;

    if (!desc.PS.pShaderBytecode || !desc.VS.pShaderBytecode)
    {
      s_cache_is_corrupted = true;
      return;
    }

    BlendingState blend_state = {};
    blend_state.hex = key.blend_state_hex;
    desc.BlendState = StateCache::GetDesc(blend_state);

    DepthState depth_stencil_state = {};
    depth_stencil_state.hex = key.depth_stencil_state_hex;
    desc.DepthStencilState = StateCache::GetDesc(depth_stencil_state);

    RasterizationState rasterizer_state = {};
    rasterizer_state.hex = key.rasterizer_state_hex;
    desc.RasterizerState = StateCache::GetDesc(rasterizer_state);

    desc.PrimitiveTopologyType = key.topology;

    // search for a cached native vertex format
    const PortableVertexDeclaration& native_vtx_decl = key.vertex_declaration;
    NativeVertexFormat* native = VertexLoaderManager::GetOrCreateMatchingFormat(native_vtx_decl);

    desc.InputLayout = static_cast<D3DVertexFormat*>(native)->GetActiveInputLayout();

    desc.CachedPSO.CachedBlobSizeInBytes = value_size;
    desc.CachedPSO.pCachedBlob = value;

    ComPtr<ID3D12PipelineState> pso;
    HRESULT hr = D3D::device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(pso.ReleaseAndGetAddressOf()));

    if (FAILED(hr))
    {
      // Failure can occur if disk cache is corrupted, or a driver upgrade invalidates the existing blobs.
      // In this case, we need to clear the disk cache.
      s_cache_is_corrupted = true;
      return;
    }

    SmallPsoDesc small_desc = {};
    small_desc.using_uber_pixel_shader = key.using_uber_pixel_shader;
    small_desc.using_uber_vertex_shader = key.using_uber_vertex_shader;
    small_desc.blend_state.hex = key.blend_state_hex;
    small_desc.depth_stencil_state.hex = key.depth_stencil_state_hex;
    small_desc.rasterizer_state.hex = key.rasterizer_state_hex;
    small_desc.gs_bytecode = desc.GS;
    small_desc.vs_bytecode = desc.VS;
    small_desc.ps_bytecode = desc.PS;
    small_desc.hs_bytecode = desc.HS;
    small_desc.ds_bytecode = desc.DS;
    small_desc.input_Layout = static_cast<D3DVertexFormat*>(native);
    small_desc.sample_count = key.sample_desc.Count;
    small_desc.rtformat = key.rtformat;
    s_gx_state_cache.m_small_pso_map[small_desc] = pso;
  }
};

StateCache::StateCache()
{
  m_current_pso_desc = {};

  m_current_pso_desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // This state changes in PSTextureEncoder::Encode.
  m_current_pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT; // This state changes in PSTextureEncoder::Encode.
  m_current_pso_desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFF;
  m_current_pso_desc.NumRenderTargets = 1;
  m_current_pso_desc.SampleMask = UINT_MAX;
  m_current_pso_desc.SampleDesc.Count = g_ActiveConfig.iMultisamples;
  m_current_pso_desc.SampleDesc.Quality = 0;
  m_enable_disk_cache = true;
}

void StateCache::LoadFromDisk()
{
  std::string cache_filename = GetDiskShaderCacheFileName(API_D3D11, "pso", true, true);

  PipelineStateCacheInserter inserter;
  s_pso_disk_cache.OpenAndRead(cache_filename, inserter);

  if (s_cache_is_corrupted)
  {
    // If a PSO fails to create, that means either:
    // - The file itself is corrupt.
    // - A driver/HW change has occured, causing the existing cache blobs to be invalid.
    // 
    // In either case, we want to re-create the disk cache. This should not be a frequent occurence.

    s_pso_disk_cache.Close();

    s_gx_state_cache.m_small_pso_map.clear();

    File::Delete(cache_filename);

    s_pso_disk_cache.OpenAndRead(cache_filename, inserter);

    s_cache_is_corrupted = false;
  }
}

void StateCache::Reload()
{
  m_small_pso_map.clear();

  s_pso_disk_cache.Sync();
  s_pso_disk_cache.Close();
  LoadFromDisk();
}

void StateCache::Init()
{
  if (!s_gx_state_cache.m_enable_disk_cache)
  {
    return;
  }

  LoadFromDisk();
}

void StateCache::CheckDiskCacheState(IDXGIAdapter* adapter)
{
  DXGI_ADAPTER_DESC adapter_desc = {};
  adapter->GetDesc(&adapter_desc);
  s_gx_state_cache.m_enable_disk_cache = true;

  // Disable disk cache for drivers that have issues when recreating
  // identical PSOs from the cache blob.
  if (adapter_desc.VendorId == 0x1002)        // Microsoft WARP
  {
    s_gx_state_cache.m_enable_disk_cache = false;
  }
}

D3D12_SAMPLER_DESC StateCache::GetDesc(SamplerState state)
{
  D3D12_SAMPLER_DESC sampdc;
  sampdc.MaxAnisotropy = 1;
  if (state.anisotropic_filtering)
  {
    sampdc.MaxAnisotropy = 1u << g_ActiveConfig.iMaxAnisotropy;
  }
  if (state.anisotropic_filtering && state.mipmap_filter == SamplerState::Filter::Linear)
  {
    sampdc.Filter = D3D12_FILTER_ANISOTROPIC;
  }
  else if (state.mipmap_filter == SamplerState::Filter::Linear)
  {
    if (state.min_filter == SamplerState::Filter::Linear)
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D12_FILTER_MIN_MAG_MIP_LINEAR :
      D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
    else
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR :
      D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
  }
  else
  {
    if (state.min_filter == SamplerState::Filter::Linear)
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT :
      D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    else
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT :
      D3D12_FILTER_MIN_MAG_MIP_POINT;
  }

  static constexpr std::array<D3D12_TEXTURE_ADDRESS_MODE, 3> address_modes = {
    { D3D12_TEXTURE_ADDRESS_MODE_CLAMP, D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_MIRROR } };
  sampdc.AddressU = address_modes[static_cast<u32>(state.wrap_u.Value())];
  sampdc.AddressV = address_modes[static_cast<u32>(state.wrap_v.Value())];
  sampdc.MaxLOD = state.max_lod / 16.f;
  sampdc.MinLOD = state.min_lod / 16.f;
  sampdc.MipLODBias = (s32)state.lod_bias / 256.f;
  sampdc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
  sampdc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
  sampdc.BorderColor[0] = sampdc.BorderColor[1] = sampdc.BorderColor[2] = sampdc.BorderColor[3] = 1.0f;
  return sampdc;
}

static constexpr std::array<D3D12_LOGIC_OP, 16> logic_ops = {
  { D3D12_LOGIC_OP_CLEAR, D3D12_LOGIC_OP_AND, D3D12_LOGIC_OP_AND_REVERSE, D3D12_LOGIC_OP_COPY,
  D3D12_LOGIC_OP_AND_INVERTED, D3D12_LOGIC_OP_NOOP, D3D12_LOGIC_OP_XOR, D3D12_LOGIC_OP_OR,
  D3D12_LOGIC_OP_NOR, D3D12_LOGIC_OP_EQUIV, D3D12_LOGIC_OP_INVERT, D3D12_LOGIC_OP_OR_REVERSE,
  D3D12_LOGIC_OP_COPY_INVERTED, D3D12_LOGIC_OP_OR_INVERTED, D3D12_LOGIC_OP_NAND,
  D3D12_LOGIC_OP_SET } };

// fallbacks for devices that does not support logic blending
static constexpr std::array<D3D12_BLEND_OP, 16> d3dLogicOps =
{
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_SUBTRACT,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_REV_SUBTRACT,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_MAX,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_MAX,
  D3D12_BLEND_OP_MAX,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_ADD,
  D3D12_BLEND_OP_ADD
};
static constexpr std::array<D3D12_BLEND, 16> d3dLogicOpSrcFactors =
{
  D3D12_BLEND_ZERO,
  D3D12_BLEND_DEST_COLOR,
  D3D12_BLEND_ONE,
  D3D12_BLEND_ONE,
  D3D12_BLEND_DEST_COLOR,
  D3D12_BLEND_ZERO,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_ONE,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_ONE
};
static constexpr std::array<D3D12_BLEND, 16> d3dLogicOpDestFactors =
{
  D3D12_BLEND_ZERO,
  D3D12_BLEND_ZERO,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_ZERO,
  D3D12_BLEND_ONE,
  D3D12_BLEND_ONE,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_ONE,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_SRC_COLOR,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_INV_DEST_COLOR,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_ONE,
  D3D12_BLEND_INV_SRC_COLOR,
  D3D12_BLEND_ONE
};

D3D12_BLEND_DESC StateCache::GetDesc(BlendingState state)
{
  D3D12_BLEND_DESC blenddc = {};
  D3D12_RENDER_TARGET_BLEND_DESC& tdesc = blenddc.RenderTarget[0];
  if (state.logicopenable)
  {
    if (D3D::GetLogicOpSupported())
    {
      static constexpr std::array<D3D12_LOGIC_OP, 16> logic_op = {
        { D3D12_LOGIC_OP_CLEAR, D3D12_LOGIC_OP_AND, D3D12_LOGIC_OP_AND_REVERSE, D3D12_LOGIC_OP_COPY,
        D3D12_LOGIC_OP_AND_INVERTED, D3D12_LOGIC_OP_NOOP, D3D12_LOGIC_OP_XOR, D3D12_LOGIC_OP_OR,
        D3D12_LOGIC_OP_NOR, D3D12_LOGIC_OP_EQUIV, D3D12_LOGIC_OP_INVERT, D3D12_LOGIC_OP_OR_REVERSE,
        D3D12_LOGIC_OP_COPY_INVERTED, D3D12_LOGIC_OP_OR_INVERTED, D3D12_LOGIC_OP_NAND,
        D3D12_LOGIC_OP_SET } };
      tdesc.LogicOpEnable = TRUE;
      tdesc.LogicOp = logic_op[state.logicmode];
    }
    else
    {
      tdesc.BlendEnable = true;
      tdesc.SrcBlend = d3dLogicOpSrcFactors[state.logicmode.Value()];
      tdesc.DestBlend = d3dLogicOpDestFactors[state.logicmode.Value()];
      tdesc.BlendOp = d3dLogicOps[state.logicmode.Value()];
      tdesc.BlendOpAlpha = tdesc.BlendOp;

      if (tdesc.SrcBlend == D3D12_BLEND_SRC_COLOR)
        tdesc.SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
      else if (tdesc.SrcBlend == D3D12_BLEND_INV_SRC_COLOR)
        tdesc.SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
      else if (tdesc.SrcBlend == D3D12_BLEND_DEST_COLOR)
        tdesc.SrcBlendAlpha = D3D12_BLEND_DEST_ALPHA;
      else if (tdesc.SrcBlend == D3D12_BLEND_INV_DEST_COLOR)
        tdesc.SrcBlendAlpha = D3D12_BLEND_INV_DEST_ALPHA;
      else
        tdesc.SrcBlendAlpha = tdesc.SrcBlend;

      if (tdesc.DestBlend == D3D12_BLEND_SRC_COLOR)
        tdesc.DestBlendAlpha = D3D12_BLEND_SRC_ALPHA;
      else if (tdesc.DestBlend == D3D12_BLEND_INV_SRC_COLOR)
        tdesc.DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
      else if (tdesc.DestBlend == D3D12_BLEND_DEST_COLOR)
        tdesc.DestBlendAlpha = D3D12_BLEND_DEST_ALPHA;
      else if (tdesc.DestBlend == D3D12_BLEND_INV_DEST_COLOR)
        tdesc.DestBlendAlpha = D3D12_BLEND_INV_DEST_ALPHA;
      else
        tdesc.DestBlendAlpha = tdesc.DestBlend;
    }
  }
  else
  {
    tdesc.BlendEnable = state.blendenable;
    if (state.blendenable)
    {
      const bool use_dual_source = state.usedualsrc;
      const std::array<D3D12_BLEND, 8> src_factors = {
        { D3D12_BLEND_ZERO, D3D12_BLEND_ONE, D3D12_BLEND_DEST_COLOR, D3D12_BLEND_INV_DEST_COLOR,
        use_dual_source ? D3D12_BLEND_SRC1_ALPHA : D3D12_BLEND_SRC_ALPHA,
        use_dual_source ? D3D12_BLEND_INV_SRC1_ALPHA : D3D12_BLEND_INV_SRC_ALPHA,
        D3D12_BLEND_DEST_ALPHA, D3D12_BLEND_INV_DEST_ALPHA } };
      const std::array<D3D12_BLEND, 8> dst_factors = {
        { D3D12_BLEND_ZERO, D3D12_BLEND_ONE, D3D12_BLEND_SRC_COLOR, D3D12_BLEND_INV_SRC_COLOR,
        use_dual_source ? D3D12_BLEND_SRC1_ALPHA : D3D12_BLEND_SRC_ALPHA,
        use_dual_source ? D3D12_BLEND_INV_SRC1_ALPHA : D3D12_BLEND_INV_SRC_ALPHA,
        D3D12_BLEND_DEST_ALPHA, D3D12_BLEND_INV_DEST_ALPHA } };

      tdesc.SrcBlend = src_factors[state.srcfactor];
      tdesc.SrcBlendAlpha = src_factors[state.srcfactoralpha];
      tdesc.DestBlend = dst_factors[state.dstfactor];
      tdesc.DestBlendAlpha = dst_factors[state.dstfactoralpha];
      tdesc.BlendOp = state.subtract ? D3D12_BLEND_OP_REV_SUBTRACT : D3D12_BLEND_OP_ADD;
      tdesc.BlendOpAlpha = state.subtractAlpha ? D3D12_BLEND_OP_REV_SUBTRACT : D3D12_BLEND_OP_ADD;
    }
  }

  if (state.colorupdate)
    tdesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_RED | D3D12_COLOR_WRITE_ENABLE_GREEN |
    D3D12_COLOR_WRITE_ENABLE_BLUE;
  else
    tdesc.RenderTargetWriteMask = 0;
  if (state.alphaupdate)
    tdesc.RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
  return blenddc;
}

D3D12_RASTERIZER_DESC StateCache::GetDesc(RasterizationState state)
{
  static constexpr std::array<D3D12_CULL_MODE, 4> cull_modes = {
    { D3D12_CULL_MODE_NONE, D3D12_CULL_MODE_BACK, D3D12_CULL_MODE_FRONT, D3D12_CULL_MODE_BACK } };
  return{
      D3D12_FILL_MODE_SOLID,
      cull_modes[state.cullmode],
      false,
      0,
      0.f,
      0.0f,
      false,
      true,
      false,
      0,
      D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
  };
}

inline D3D12_DEPTH_STENCIL_DESC StateCache::GetDesc(DepthState state)
{
  D3D12_DEPTH_STENCIL_DESC depthdc;

  depthdc.StencilEnable = FALSE;
  depthdc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
  depthdc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

  D3D12_DEPTH_STENCILOP_DESC defaultStencilOp = { D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
  depthdc.FrontFace = defaultStencilOp;
  depthdc.BackFace = defaultStencilOp;

  const D3D12_COMPARISON_FUNC d3dCmpFuncs[8] =
  {
      D3D12_COMPARISON_FUNC_NEVER,
      D3D12_COMPARISON_FUNC_GREATER,
      D3D12_COMPARISON_FUNC_EQUAL,
      D3D12_COMPARISON_FUNC_GREATER_EQUAL,
      D3D12_COMPARISON_FUNC_LESS,
      D3D12_COMPARISON_FUNC_NOT_EQUAL,
      D3D12_COMPARISON_FUNC_LESS_EQUAL,
      D3D12_COMPARISON_FUNC_ALWAYS
  };

  const D3D12_COMPARISON_FUNC d3dInvCmpFuncs[8] =
  {
      D3D12_COMPARISON_FUNC_NEVER,
      D3D12_COMPARISON_FUNC_LESS_EQUAL,
      D3D12_COMPARISON_FUNC_EQUAL,
      D3D12_COMPARISON_FUNC_LESS,
      D3D12_COMPARISON_FUNC_GREATER_EQUAL,
      D3D12_COMPARISON_FUNC_NOT_EQUAL,
      D3D12_COMPARISON_FUNC_GREATER,
      D3D12_COMPARISON_FUNC_ALWAYS
  };

  if (state.testenable)
  {
    depthdc.DepthEnable = TRUE;
    depthdc.DepthWriteMask = state.updateenable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
    depthdc.DepthFunc = state.reversed_depth ? d3dInvCmpFuncs[state.func] : d3dCmpFuncs[state.func];
  }
  else
  {
    // if the test is disabled write is disabled too
    depthdc.DepthEnable = FALSE;
    depthdc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
  }

  return depthdc;
}

HRESULT StateCache::GetPipelineStateObjectFromCache(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& pso_desc, ID3D12PipelineState** pso)
{
  auto it = m_pso_map.find(pso_desc);

  if (it == m_pso_map.end())
  {
    // Not found, create new PSO.

    ComPtr<ID3D12PipelineState> new_pso;
    HRESULT hr = D3D::device->CreateGraphicsPipelineState(&pso_desc, IID_PPV_ARGS(new_pso.ReleaseAndGetAddressOf()));

    if (FAILED(hr))
    {
      return hr;
    }

    m_pso_map[pso_desc] = new_pso;
    *pso = new_pso.Get();
  }
  else
  {
    *pso = it->second.Get();
  }

  return S_OK;
}

HRESULT StateCache::GetPipelineStateObjectFromCache(const SmallPsoDesc& pso_desc, ID3D12PipelineState** pso, D3D12_PRIMITIVE_TOPOLOGY_TYPE topology)
{
  auto it = m_small_pso_map.find(pso_desc);

  if (it == m_small_pso_map.end())
  {
    // Not found, create new PSO.

    // RootSignature, SampleMask, NumRenderTargets, RTVFormats, DSVFormat
    // never change so they are set in constructor and forgotten.
    m_current_pso_desc.GS = pso_desc.gs_bytecode;
    m_current_pso_desc.PS = pso_desc.ps_bytecode;
    m_current_pso_desc.VS = pso_desc.vs_bytecode;
    m_current_pso_desc.HS = pso_desc.hs_bytecode;
    m_current_pso_desc.DS = pso_desc.ds_bytecode;
    m_current_pso_desc.RTVFormats[0] = pso_desc.rtformat;
    m_current_pso_desc.pRootSignature = D3D::GetRootSignature();

    m_current_pso_desc.BlendState = GetDesc(pso_desc.blend_state);
    m_current_pso_desc.DepthStencilState = GetDesc(pso_desc.depth_stencil_state);
    m_current_pso_desc.RasterizerState = GetDesc(pso_desc.rasterizer_state);
    m_current_pso_desc.PrimitiveTopologyType = topology;
    m_current_pso_desc.InputLayout = pso_desc.input_Layout->GetActiveInputLayout();
    m_current_pso_desc.SampleDesc.Count = pso_desc.sample_count;

    ComPtr<ID3D12PipelineState> new_pso;
    HRESULT hr = D3D::device->CreateGraphicsPipelineState(&m_current_pso_desc, IID_PPV_ARGS(new_pso.ReleaseAndGetAddressOf()));

    if (FAILED(hr))
    {
      CheckHR(hr);
      return hr;
    }

    m_small_pso_map[pso_desc] = new_pso;
    *pso = new_pso.Get();

    if (m_enable_disk_cache)
    {
      // This contains all of the information needed to reconstruct a PSO at startup.
      SmallPsoDiskDesc disk_desc = {};
      disk_desc.using_uber_pixel_shader = pso_desc.using_uber_pixel_shader;
      disk_desc.using_uber_vertex_shader = pso_desc.using_uber_vertex_shader;
      disk_desc.root_signature_index = static_cast<u32>(D3D::GetRootSignatureIndex());
      disk_desc.blend_state_hex = pso_desc.blend_state.hex;
      disk_desc.depth_stencil_state_hex = pso_desc.depth_stencil_state.hex;
      disk_desc.rasterizer_state_hex = pso_desc.rasterizer_state.hex;
      disk_desc.gs_uid = ShaderCache::GetActiveGeometryShaderUid();
      if (pso_desc.using_uber_pixel_shader)
      {
        disk_desc.pus_uid = ShaderCache::GetActivePixelUberShaderUid();
      }
      else
      {
        disk_desc.ps_uid = ShaderCache::GetActivePixelShaderUid();
      }
      if (pso_desc.using_uber_vertex_shader)
      {
        disk_desc.vus_uid = ShaderCache::GetActiveVertexUberShaderUid();
      }
      else
      {
        disk_desc.vs_uid = ShaderCache::GetActiveVertexShaderUid();
      }
      
      disk_desc.hds_uid = ShaderCache::GetActiveTessellationShaderUid();
      disk_desc.vertex_declaration = pso_desc.input_Layout->GetVertexDeclaration();
      disk_desc.topology = topology;
      disk_desc.sample_desc.Count = g_ActiveConfig.iMultisamples;
      disk_desc.rtformat = pso_desc.rtformat;
      // This shouldn't fail.. but if it does, don't cache to disk.
      ComPtr<ID3DBlob> psoBlob;
      hr = new_pso->GetCachedBlob(psoBlob.ReleaseAndGetAddressOf());
      if (SUCCEEDED(hr))
      {
        s_pso_disk_cache.Append(disk_desc, reinterpret_cast<const u8*>(psoBlob->GetBufferPointer()), static_cast<u32>(psoBlob->GetBufferSize()));
      }
    }
  }
  else
  {
    *pso = it->second.Get();
  }

  return S_OK;
}

void StateCache::Clear()
{
  m_pso_map.clear();
  m_small_pso_map.clear();

  s_pso_disk_cache.Sync();
  s_pso_disk_cache.Close();
}
}  // namespace DX12
