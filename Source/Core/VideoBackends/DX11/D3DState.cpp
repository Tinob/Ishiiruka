// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Common/Logging/Log.h"
#include "Common/CommonTypes.h"
#include "Common/MsgHandler.h"

#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DState.h"

#include "VideoCommon/RenderBase.h"
#include "VideoCommon/VideoConfig.h"

namespace DX11
{

namespace D3D
{

StateManager* stateman;

template<typename T> AutoState<T>::AutoState(const T* object) : state(object)
{
  ((IUnknown*)state)->AddRef();
}

template<typename T> AutoState<T>::AutoState(const AutoState<T> &source)
{
  state = source.get();
  ((T*)state)->AddRef();
}

template<typename T> AutoState<T>::~AutoState()
{
  if (state) ((T*)state)->Release();
  state = nullptr;
}

StateManager::StateManager()
  : m_currentBlendState(nullptr)
  , m_currentDepthState(nullptr)
  , m_currentRasterizerState(nullptr)
  , m_dirtyFlags(~0u)
  , m_pending()
  , m_current()
{
  use_partial_buffer_update = D3D::SupportPartialContantBufferUpdate();
}

void StateManager::PushBlendState(const ID3D11BlendState* state)
{
  m_blendStates.push(AutoBlendState(state));
}
void StateManager::PushDepthState(const ID3D11DepthStencilState* state)
{
  m_depthStates.push(AutoDepthStencilState(state));
}
void StateManager::PushRasterizerState(const ID3D11RasterizerState* state)
{
  m_rasterizerStates.push(AutoRasterizerState(state));
}
void StateManager::PopBlendState()
{
  m_blendStates.pop();
}
void StateManager::PopDepthState()
{
  m_depthStates.pop();
}
void StateManager::PopRasterizerState()
{
  m_rasterizerStates.pop();
}

void StateManager::Apply()
{
  if (!m_blendStates.empty())
  {
    if (m_currentBlendState != m_blendStates.top().get())
    {
      m_currentBlendState = (ID3D11BlendState*)m_blendStates.top().get();
      D3D::context->OMSetBlendState(m_currentBlendState, nullptr, 0xFFFFFFFF);
    }
  }
  else ERROR_LOG(VIDEO, "Tried to apply without blend state!");

  if (!m_depthStates.empty())
  {
    if (m_currentDepthState != m_depthStates.top().get())
    {
      m_currentDepthState = (ID3D11DepthStencilState*)m_depthStates.top().get();
      D3D::context->OMSetDepthStencilState(m_currentDepthState, 0);
    }
  }
  else ERROR_LOG(VIDEO, "Tried to apply without depth state!");

  if (!m_rasterizerStates.empty())
  {
    if (m_currentRasterizerState != m_rasterizerStates.top().get())
    {
      m_currentRasterizerState = (ID3D11RasterizerState*)m_rasterizerStates.top().get();
      D3D::context->RSSetState(m_currentRasterizerState);
    }
  }
  else ERROR_LOG(VIDEO, "Tried to apply without rasterizer state!");

  if (!m_dirtyFlags)
  {
    return;
  }

  if (m_dirtyFlags & DirtyFlag_Constants)
  {
    if (use_partial_buffer_update)
    {
      if (m_dirtyFlags & DirtyFlag_PixelConstants)
      {
        if (m_pending.pixelConstantsSize[0] == 0 && m_pending.pixelConstantsSize[1] == 0)
        {
          D3D::context->PSSetConstantBuffers(0, m_pending.pixelConstants[1] ? 2 : 1, m_pending.pixelConstants);
        }
        else
        {
          D3D::context1->PSSetConstantBuffers1(0, m_pending.pixelConstants[1] ? 2 : 1, m_pending.pixelConstants, m_pending.pixelConstantsOffset, m_pending.pixelConstantsSize);
        }
        m_current.pixelConstants[0] = m_pending.pixelConstants[0];
        m_current.pixelConstantsOffset[0] = m_pending.pixelConstantsOffset[0];
        m_current.pixelConstantsSize[0] = m_pending.pixelConstantsSize[0];
        m_current.pixelConstants[1] = m_pending.pixelConstants[1];
        m_current.pixelConstantsOffset[1] = m_pending.pixelConstantsOffset[1];
        m_current.pixelConstantsSize[1] = m_pending.pixelConstantsSize[1];
      }
      if (m_dirtyFlags & DirtyFlag_VertexConstants)
      {
        if (m_pending.vertexConstantsSize == 0)
        {
          D3D::context->VSSetConstantBuffers(0, 1, &m_pending.vertexConstants);
        }
        else
        {
          D3D::context1->VSSetConstantBuffers1(0, 1, &m_pending.vertexConstants, &m_pending.vertexConstantsOffset, &m_pending.vertexConstantsSize);
        }
        m_current.vertexConstants = m_pending.vertexConstants;
        m_current.vertexConstantsOffset = m_pending.vertexConstantsOffset;
        m_current.vertexConstantsSize = m_pending.vertexConstantsSize;
      }
      if (m_dirtyFlags & DirtyFlag_GeometryConstants)
      {
        if (m_pending.geometryConstantsSize == 0)
        {
          D3D::context->GSSetConstantBuffers(0, 1, &m_pending.geometryConstants);
        }
        else
        {
          D3D::context1->GSSetConstantBuffers1(0, 1, &m_pending.geometryConstants, &m_pending.geometryConstantsOffset, &m_pending.geometryConstantsSize);
        }
        m_current.geometryConstants = m_pending.geometryConstants;
        m_current.geometryConstantsOffset = m_pending.geometryConstantsOffset;
        m_current.geometryConstantsSize = m_pending.geometryConstantsSize;
      }
      if (m_dirtyFlags & DirtyFlag_HullDomainConstants)
      {
        if (g_ActiveConfig.backend_info.bSupportsTessellation)
        {
          if (m_pending.hulldomainConstantsSize == 0)
          {
            D3D::context->HSSetConstantBuffers(0, 3, m_pending.hulldomainConstants);
            D3D::context->DSSetConstantBuffers(0, 3, m_pending.hulldomainConstants);
          }
          else
          {
            D3D::context1->HSSetConstantBuffers1(0, 3, m_pending.hulldomainConstants, m_pending.hulldomainConstantsOffset, m_pending.hulldomainConstantsSize);
            D3D::context1->DSSetConstantBuffers1(0, 3, m_pending.hulldomainConstants, m_pending.hulldomainConstantsOffset, m_pending.hulldomainConstantsSize);
          }
        }
        m_current.hulldomainConstants[0] = m_pending.hulldomainConstants[0];
        m_current.hulldomainConstantsOffset[0] = m_pending.hulldomainConstantsOffset[0];
        m_current.hulldomainConstantsSize[0] = m_pending.hulldomainConstantsSize[0];
        m_current.hulldomainConstants[1] = m_pending.hulldomainConstants[1];
        m_current.hulldomainConstantsOffset[1] = m_pending.hulldomainConstantsOffset[1];
        m_current.hulldomainConstantsSize[1] = m_pending.hulldomainConstantsSize[1];
        m_current.hulldomainConstants[2] = m_pending.hulldomainConstants[2];
        m_current.hulldomainConstantsOffset[2] = m_pending.hulldomainConstantsOffset[2];
        m_current.hulldomainConstantsSize[2] = m_pending.hulldomainConstantsSize[2];
      }
    }
    else
    {
      if (m_dirtyFlags & DirtyFlag_PixelConstants)
      {
        D3D::context->PSSetConstantBuffers(0, m_pending.pixelConstants[1] ? 2 : 1, m_pending.pixelConstants);
        m_current.pixelConstants[0] = m_pending.pixelConstants[0];
        m_current.pixelConstants[1] = m_pending.pixelConstants[1];
      }
      if (m_dirtyFlags & DirtyFlag_VertexConstants)
      {
        D3D::context->VSSetConstantBuffers(0, 1, &m_pending.vertexConstants);
        m_current.vertexConstants = m_pending.vertexConstants;
      }
      if (m_dirtyFlags & DirtyFlag_GeometryConstants)
      {
        D3D::context->GSSetConstantBuffers(0, 1, &m_pending.geometryConstants);
        m_current.geometryConstants = m_pending.geometryConstants;
      }
      if (m_dirtyFlags & DirtyFlag_HullDomainConstants)
      {
        if (g_ActiveConfig.backend_info.bSupportsTessellation)
        {
          D3D::context->HSSetConstantBuffers(0, 3, m_pending.hulldomainConstants);
          D3D::context->DSSetConstantBuffers(0, 3, m_pending.hulldomainConstants);
        }
        m_current.hulldomainConstants[0] = m_pending.hulldomainConstants[0];
        m_current.hulldomainConstants[1] = m_pending.hulldomainConstants[1];
        m_current.hulldomainConstants[2] = m_pending.hulldomainConstants[2];
      }
    }
  }

  if (m_dirtyFlags & (DirtyFlag_Buffers | DirtyFlag_InputAssembler))
  {
    if (m_dirtyFlags & DirtyFlag_VertexBuffer)
    {
      D3D::context->IASetVertexBuffers(0, 1, &m_pending.vertexBuffer, &m_pending.vertexBufferStride, &m_pending.vertexBufferOffset);
      m_current.vertexBuffer = m_pending.vertexBuffer;
      m_current.vertexBufferStride = m_pending.vertexBufferStride;
      m_current.vertexBufferOffset = m_pending.vertexBufferOffset;
    }

    if (m_dirtyFlags & DirtyFlag_IndexBuffer)
    {
      D3D::context->IASetIndexBuffer(m_pending.indexBuffer, DXGI_FORMAT_R16_UINT, 0);
      m_current.indexBuffer = m_pending.indexBuffer;
    }

    if (m_current.topology != m_pending.topology)
    {
      D3D::context->IASetPrimitiveTopology(m_pending.topology);
      m_current.topology = m_pending.topology;
    }

    if (m_current.inputLayout != m_pending.inputLayout)
    {
      D3D::context->IASetInputLayout(m_pending.inputLayout);
      m_current.inputLayout = m_pending.inputLayout;
    }
  }
  u64 dirty_elements = m_dirtyFlags & DirtyFlag_Textures;
  if (dirty_elements)
  {
    while (dirty_elements)
    {
      unsigned long index;
      _BitScanForward64(&index, dirty_elements);
      D3D::context->PSSetShaderResources(index, 1, &m_pending.textures[index]);
      D3D::context->DSSetShaderResources(index, 1, &m_pending.textures[index]);
      m_current.textures[index] = m_pending.textures[index];
      dirty_elements &= ~(1ull << index);
    }
  }
  dirty_elements = (m_dirtyFlags & DirtyFlag_Samplers) >> 16;
  if (dirty_elements)
  {
    while (dirty_elements)
    {
      unsigned long index;
      _BitScanForward64(&index, dirty_elements);
      D3D::context->PSSetSamplers(index, 1, &m_pending.samplers[index]);
      D3D::context->DSSetSamplers(index, 1, &m_pending.samplers[index]);
      m_current.samplers[index] = m_pending.samplers[index];
      dirty_elements &= ~(1 << index);
    }
  }

  if (m_dirtyFlags & DirtyFlag_Shaders)
  {
    if (m_current.pixelShader != m_pending.pixelShader)
    {
      D3D::context->PSSetShader(m_pending.pixelShader, nullptr, 0);
      m_current.pixelShader = m_pending.pixelShader;
    }

    if (m_current.vertexShader != m_pending.vertexShader)
    {
      D3D::context->VSSetShader(m_pending.vertexShader, nullptr, 0);
      m_current.vertexShader = m_pending.vertexShader;
    }

    if (m_current.geometryShader != m_pending.geometryShader)
    {
      D3D::context->GSSetShader(m_pending.geometryShader, nullptr, 0);
      m_current.geometryShader = m_pending.geometryShader;
    }
    if (g_ActiveConfig.backend_info.bSupportsTessellation)
    {
      if (m_current.hullShader != m_pending.hullShader)
      {
        D3D::context->HSSetShader(m_pending.hullShader, nullptr, 0);
        m_current.hullShader = m_pending.hullShader;
      }

      if (m_current.domainShader != m_pending.domainShader)
      {
        D3D::context->DSSetShader(m_pending.domainShader, nullptr, 0);
        m_current.domainShader = m_pending.domainShader;
      }
    }
  }

  m_dirtyFlags = 0;
}

u64 StateManager::UnsetTexture(ID3D11ShaderResourceView* srv)
{
  u64 mask = 0;

  for (u32 index = 0; index < 16; ++index)
  {
    if (m_current.textures[index] == srv || m_pending.textures[index] == srv)
    {
      SetTexture(index, nullptr);
      mask |= 1ull << index;
    }
  }
  return mask;
}

void StateManager::SetTextureByMask(u64 textureSlotMask, ID3D11ShaderResourceView* srv)
{
  while (textureSlotMask)
  {
    unsigned long index;
    _BitScanForward64(&index, textureSlotMask);
    SetTexture(index, srv);
    textureSlotMask &= ~(1ull << index);
  }
}

}  // namespace D3D

ID3D11SamplerState* StateCache::Get(SamplerState state)
{
  auto it = m_sampler.find(state.hex);
  if (it != m_sampler.end())
    return it->second.get();

  D3D11_SAMPLER_DESC sampdc = CD3D11_SAMPLER_DESC(CD3D11_DEFAULT());
  if (state.anisotropic_filtering)
  {
    sampdc.MaxAnisotropy = 1u << g_ActiveConfig.iMaxAnisotropy;
  }
  if (state.anisotropic_filtering && state.mipmap_filter == SamplerState::Filter::Linear)
  {
    sampdc.Filter = D3D11_FILTER_ANISOTROPIC;
  }
  else if (state.mipmap_filter == SamplerState::Filter::Linear)
  {
    if (state.min_filter == SamplerState::Filter::Linear)
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D11_FILTER_MIN_MAG_MIP_LINEAR :
      D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
    else
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR :
      D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
  }
  else
  {
    if (state.min_filter == SamplerState::Filter::Linear)
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT :
      D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
    else
      sampdc.Filter = (state.mag_filter == SamplerState::Filter::Linear) ?
      D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT :
      D3D11_FILTER_MIN_MAG_MIP_POINT;
  }

  static constexpr std::array<D3D11_TEXTURE_ADDRESS_MODE, 3> address_modes = {
    { D3D11_TEXTURE_ADDRESS_CLAMP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MIRROR } };
  sampdc.AddressU = address_modes[static_cast<u32>(state.wrap_u.Value())];
  sampdc.AddressV = address_modes[static_cast<u32>(state.wrap_v.Value())];
  sampdc.MaxLOD = state.max_lod / 16.f;
  sampdc.MinLOD = state.min_lod / 16.f;
  sampdc.MipLODBias = (s32)state.lod_bias / 256.f;


  ID3D11SamplerState* res = nullptr;
  HRESULT hr = D3D::device->CreateSamplerState(&sampdc, &res);
  if (FAILED(hr))
    PanicAlert("Fail %s %d\n", __FILE__, __LINE__);

  D3D::SetDebugObjectName(res, "sampler state used to emulate the GX pipeline");
  m_sampler.emplace(state.hex, std::move(D3D::SamplerStatePtr(res)));
  return res;
}

static constexpr std::array<D3D11_LOGIC_OP, 16> logic_ops = {
  { D3D11_LOGIC_OP_CLEAR, D3D11_LOGIC_OP_AND, D3D11_LOGIC_OP_AND_REVERSE, D3D11_LOGIC_OP_COPY,
  D3D11_LOGIC_OP_AND_INVERTED, D3D11_LOGIC_OP_NOOP, D3D11_LOGIC_OP_XOR, D3D11_LOGIC_OP_OR,
  D3D11_LOGIC_OP_NOR, D3D11_LOGIC_OP_EQUIV, D3D11_LOGIC_OP_INVERT, D3D11_LOGIC_OP_OR_REVERSE,
  D3D11_LOGIC_OP_COPY_INVERTED, D3D11_LOGIC_OP_OR_INVERTED, D3D11_LOGIC_OP_NAND,
  D3D11_LOGIC_OP_SET } };

// fallbacks for devices that does not support logic blending
static constexpr std::array<D3D11_BLEND_OP, 16> d3dLogicOps =
{
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_SUBTRACT,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_REV_SUBTRACT,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_MAX,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_MAX,
  D3D11_BLEND_OP_MAX,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_ADD,
  D3D11_BLEND_OP_ADD
};
static constexpr std::array<D3D11_BLEND, 16> d3dLogicOpSrcFactors =
{
  D3D11_BLEND_ZERO,
  D3D11_BLEND_DEST_COLOR,
  D3D11_BLEND_ONE,
  D3D11_BLEND_ONE,
  D3D11_BLEND_DEST_COLOR,
  D3D11_BLEND_ZERO,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_ONE,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_ONE
};
static constexpr std::array<D3D11_BLEND, 16> d3dLogicOpDestFactors =
{
  D3D11_BLEND_ZERO,
  D3D11_BLEND_ZERO,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_ZERO,
  D3D11_BLEND_ONE,
  D3D11_BLEND_ONE,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_ONE,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_SRC_COLOR,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_INV_DEST_COLOR,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_ONE,
  D3D11_BLEND_INV_SRC_COLOR,
  D3D11_BLEND_ONE
};

ID3D11BlendState* StateCache::Get(BlendingState state)
{
  auto it = m_blend.find(state.hex);

  if (it != m_blend.end())
    return it->second.get();

  D3D11_BLEND_DESC blenddc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
  blenddc.AlphaToCoverageEnable = FALSE;
  blenddc.IndependentBlendEnable = FALSE;
  D3D11_RENDER_TARGET_BLEND_DESC& tdesc = blenddc.RenderTarget[0];
  
  if (state.logicopenable)
  {
    if (D3D::GetLogicOpSupported())
    {
      D3D11_BLEND_DESC1 blenddc1 = {};
      D3D11_RENDER_TARGET_BLEND_DESC1& tdesc1 = blenddc1.RenderTarget[0];
      if (state.colorupdate)
        tdesc1.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN |
        D3D11_COLOR_WRITE_ENABLE_BLUE;
      else
        tdesc1.RenderTargetWriteMask = 0;
      if (state.alphaupdate)
        tdesc.RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

      static constexpr std::array<D3D11_LOGIC_OP, 16> logic_o = {
        { D3D11_LOGIC_OP_CLEAR, D3D11_LOGIC_OP_AND, D3D11_LOGIC_OP_AND_REVERSE, D3D11_LOGIC_OP_COPY,
        D3D11_LOGIC_OP_AND_INVERTED, D3D11_LOGIC_OP_NOOP, D3D11_LOGIC_OP_XOR, D3D11_LOGIC_OP_OR,
        D3D11_LOGIC_OP_NOR, D3D11_LOGIC_OP_EQUIV, D3D11_LOGIC_OP_INVERT, D3D11_LOGIC_OP_OR_REVERSE,
        D3D11_LOGIC_OP_COPY_INVERTED, D3D11_LOGIC_OP_OR_INVERTED, D3D11_LOGIC_OP_NAND,
        D3D11_LOGIC_OP_SET } };
      tdesc1.LogicOpEnable = TRUE;
      tdesc1.LogicOp = logic_o[state.logicmode];
      ID3D11BlendState1* res = nullptr;

      HRESULT hr = D3D::device1->CreateBlendState1(&blenddc1, &res);
      if (SUCCEEDED(hr))
        D3D::SetDebugObjectName((ID3D11DeviceChild*)res, "blend state used to emulate the GX pipeline");
      else
        PanicAlert("Failed to create blend state at %s %d\n", __FILE__, __LINE__);

      m_blend.emplace(state.hex, std::move(D3D::BlendStatePtr(res)));
      return res;
    }
    else
    {
      tdesc.BlendEnable = true;
      tdesc.SrcBlend = d3dLogicOpSrcFactors[state.logicmode.Value()];      
      tdesc.DestBlend = d3dLogicOpDestFactors[state.logicmode.Value()];
      tdesc.BlendOp = d3dLogicOps[state.logicmode.Value()];
      tdesc.BlendOpAlpha = tdesc.BlendOp;

      if (tdesc.SrcBlend == D3D11_BLEND_SRC_COLOR)
        tdesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
      else if (tdesc.SrcBlend == D3D11_BLEND_INV_SRC_COLOR)
        tdesc.SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
      else if (tdesc.SrcBlend == D3D11_BLEND_DEST_COLOR)
        tdesc.SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
      else if (tdesc.SrcBlend == D3D11_BLEND_INV_DEST_COLOR)
        tdesc.SrcBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
      else
        tdesc.SrcBlendAlpha = tdesc.SrcBlend;

      if (tdesc.DestBlend == D3D11_BLEND_SRC_COLOR)
        tdesc.DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
      else if (tdesc.DestBlend == D3D11_BLEND_INV_SRC_COLOR)
        tdesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
      else if (tdesc.DestBlend == D3D11_BLEND_DEST_COLOR)
        tdesc.DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
      else if (tdesc.DestBlend == D3D11_BLEND_INV_DEST_COLOR)
        tdesc.DestBlendAlpha = D3D11_BLEND_INV_DEST_ALPHA;
      else
        tdesc.DestBlendAlpha = tdesc.DestBlend;
    }
  }
  else
  {
    tdesc.BlendEnable = state.blendenable;
    const bool use_dual_source = state.usedualsrc;
    const std::array<D3D11_BLEND, 8> src_factors = {
      { D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_DEST_COLOR, D3D11_BLEND_INV_DEST_COLOR,
      use_dual_source ? D3D11_BLEND_SRC1_ALPHA : D3D11_BLEND_SRC_ALPHA,
      use_dual_source ? D3D11_BLEND_INV_SRC1_ALPHA : D3D11_BLEND_INV_SRC_ALPHA,
      D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA } };
    const std::array<D3D11_BLEND, 8> dst_factors = {
      { D3D11_BLEND_ZERO, D3D11_BLEND_ONE, D3D11_BLEND_SRC_COLOR, D3D11_BLEND_INV_SRC_COLOR,
      use_dual_source ? D3D11_BLEND_SRC1_ALPHA : D3D11_BLEND_SRC_ALPHA,
      use_dual_source ? D3D11_BLEND_INV_SRC1_ALPHA : D3D11_BLEND_INV_SRC_ALPHA,
      D3D11_BLEND_DEST_ALPHA, D3D11_BLEND_INV_DEST_ALPHA } };

    tdesc.SrcBlend = src_factors[state.srcfactor];
    tdesc.SrcBlendAlpha = src_factors[state.srcfactoralpha];
    tdesc.DestBlend = dst_factors[state.dstfactor];
    tdesc.DestBlendAlpha = dst_factors[state.dstfactoralpha];
    tdesc.BlendOp = state.subtract ? D3D11_BLEND_OP_REV_SUBTRACT : D3D11_BLEND_OP_ADD;
    tdesc.BlendOpAlpha = state.subtractAlpha ? D3D11_BLEND_OP_REV_SUBTRACT : D3D11_BLEND_OP_ADD;
  }
 
  if (state.colorupdate)
    tdesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN |
    D3D11_COLOR_WRITE_ENABLE_BLUE;
  else
    tdesc.RenderTargetWriteMask = 0;
  if (state.alphaupdate)
    tdesc.RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

  ID3D11BlendState* res = nullptr;

  HRESULT hr = D3D::device->CreateBlendState(&blenddc, &res);
  if (SUCCEEDED(hr))
    D3D::SetDebugObjectName((ID3D11DeviceChild*)res, "blend state used to emulate the GX pipeline");
  else
    PanicAlert("Failed to create blend state at %s %d\n", __FILE__, __LINE__);

  m_blend.emplace(state.hex, std::move(D3D::BlendStatePtr(res)));

  return res;
}

ID3D11RasterizerState* StateCache::Get(RasterizationState state)
{
  auto it = m_raster.find(state.hex);

  if (it != m_raster.end())
    return it->second.get();

  static constexpr std::array<D3D11_CULL_MODE, 4> cull_modes = {
    { D3D11_CULL_NONE, D3D11_CULL_BACK, D3D11_CULL_FRONT, D3D11_CULL_BACK } };


  D3D11_RASTERIZER_DESC rastdc = {};
  rastdc.FillMode = D3D11_FILL_SOLID;
  rastdc.CullMode = cull_modes[state.cullmode];
  rastdc.ScissorEnable = TRUE;

  ID3D11RasterizerState* res = nullptr;

  HRESULT hr = D3D::device->CreateRasterizerState(&rastdc, &res);
  if (SUCCEEDED(hr))
    D3D::SetDebugObjectName((ID3D11DeviceChild*)res, "rasterizer state used to emulate the GX pipeline");
  else
    PanicAlert("Failed to create rasterizer state at %s %d\n", __FILE__, __LINE__);

  m_raster.emplace(state.hex, std::move(D3D::RasterizerStatePtr(res)));

  return res;
}

ID3D11DepthStencilState* StateCache::Get(DepthState state)
{
  auto it = m_depth.find(state.hex);

  if (it != m_depth.end())
    return it->second.get();

  D3D11_DEPTH_STENCIL_DESC depthdc = CD3D11_DEPTH_STENCIL_DESC(CD3D11_DEFAULT());

  depthdc.DepthEnable = TRUE;
  depthdc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
  depthdc.DepthFunc = D3D11_COMPARISON_LESS;
  depthdc.StencilEnable = FALSE;
  depthdc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
  depthdc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

  static const D3D11_COMPARISON_FUNC d3dCmpFuncs[8] =
  {
    D3D11_COMPARISON_NEVER,					D3D11_COMPARISON_GREATER, D3D11_COMPARISON_EQUAL,
    D3D11_COMPARISON_GREATER_EQUAL, D3D11_COMPARISON_LESS,		D3D11_COMPARISON_NOT_EQUAL,
    D3D11_COMPARISON_LESS_EQUAL,		D3D11_COMPARISON_ALWAYS
  };

  static const D3D11_COMPARISON_FUNC d3dRevFuncs[9] =
  {
    D3D11_COMPARISON_NEVER,		D3D11_COMPARISON_LESS_EQUAL,		D3D11_COMPARISON_EQUAL,
    D3D11_COMPARISON_LESS,		D3D11_COMPARISON_GREATER_EQUAL, D3D11_COMPARISON_NOT_EQUAL,
    D3D11_COMPARISON_GREATER, D3D11_COMPARISON_ALWAYS
  };

  if (state.testenable)
  {
    depthdc.DepthEnable = TRUE;
    depthdc.DepthWriteMask = state.updateenable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    depthdc.DepthFunc = state.reversed_depth ? d3dRevFuncs[state.func] : d3dCmpFuncs[state.func];
  }
  else
  {
    // if the test is disabled write is disabled too
    depthdc.DepthEnable = FALSE;
    depthdc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
  }

  ID3D11DepthStencilState* res = nullptr;

  HRESULT hr = D3D::device->CreateDepthStencilState(&depthdc, &res);
  if (SUCCEEDED(hr))
    D3D::SetDebugObjectName((ID3D11DeviceChild*)res, "depth-stencil state used to emulate the GX pipeline");
  else
    PanicAlert("Failed to create depth state at %s %d\n", __FILE__, __LINE__);
  m_depth.emplace(state.hex, std::move(D3D::DepthStencilStatePtr(res)));

  return res;
}

void StateCache::Clear()
{
  m_depth.clear();
  m_raster.clear();
  m_blend.clear();
  m_sampler.clear();
}

D3D11_PRIMITIVE_TOPOLOGY StateCache::GetPrimitiveTopology(PrimitiveType primitive)
{
  static constexpr std::array<D3D11_PRIMITIVE_TOPOLOGY, 3> primitives =
  {{
      D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
      D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
      D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
  }};
  return primitives[static_cast<u32>(primitive)];
}

}  // namespace DX11
