// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>
#include <cstddef>

#include "Common/Assert.h"
#include "Common/Align.h"
#include "Common/CommonTypes.h"
#include "Common/Logging/Log.h"

#include "VideoBackends/D3D12/D3DBase.h"
#include "VideoBackends/D3D12/D3DCommandListManager.h"
#include "VideoBackends/D3D12/D3DState.h"
#include "VideoBackends/D3D12/D3DTexture.h"
#include "VideoBackends/D3D12/DXTexture.h"
#include "VideoBackends/D3D12/StaticShaderCache.h"

#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/TextureConfig.h"

namespace DX12
{
DXTexture::DXTexture(const TextureConfig& tex_config) : HostTexture(tex_config)
{
  static const DXGI_FORMAT HostTextureFormat_To_DXGIFORMAT[]
  {
    DXGI_FORMAT_UNKNOWN,//PC_TEX_FMT_NONE
    DXGI_FORMAT_B8G8R8A8_UNORM,//PC_TEX_FMT_BGRA32
    DXGI_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_RGBA32
    DXGI_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_I4_AS_I8
    DXGI_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_IA4_AS_IA8
    DXGI_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_I8
    DXGI_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_IA8
    DXGI_FORMAT_B5G6R5_UNORM,//PC_TEX_FMT_RGB565
    DXGI_FORMAT_BC1_UNORM,//PC_TEX_FMT_DXT1
    DXGI_FORMAT_BC2_UNORM,//PC_TEX_FMT_DXT3
    DXGI_FORMAT_BC3_UNORM,//PC_TEX_FMT_DXT5
    DXGI_FORMAT_BC7_UNORM,//PC_TEX_FMT_BPTC
    DXGI_FORMAT_R32_FLOAT,//PC_TEX_FMT_DEPTH_FLOAT
    DXGI_FORMAT_R32_FLOAT,//PC_TEX_FMT_R_FLOAT
    DXGI_FORMAT_R16G16B16A16_FLOAT,//PC_TEX_FMT_RGBA16_FLOAT
    DXGI_FORMAT_R32G32B32A32_FLOAT,//PC_TEX_FMT_RGBA_FLOAT
  };
  DXGI_FORMAT dxgi_format = m_config.rendertarget ? DXGI_FORMAT_R8G8B8A8_UNORM : HostTextureFormat_To_DXGIFORMAT[m_config.pcformat];
  compressed = !m_config.rendertarget && TexDecoder::IsCompressed(m_config.pcformat);

  if (m_config.rendertarget)
  {
    m_texture = D3DTexture2D::Create(m_config.width, m_config.height,
      TEXTURE_BIND_FLAG_SHADER_RESOURCE | TEXTURE_BIND_FLAG_RENDER_TARGET,
      dxgi_format, 1, m_config.layers);;
  }
  else
  {
    ComPtr<ID3D12Resource> pTexture;

    D3D12_RESOURCE_DESC texdesc12 = CD3DX12_RESOURCE_DESC::Tex2D(dxgi_format,
      m_config.width, m_config.height, 1, m_config.levels);
    CD3DX12_HEAP_PROPERTIES hprop(D3D12_HEAP_TYPE_DEFAULT);
    CheckHR(
      D3D::device->CreateCommittedResource(
        &hprop,
        D3D12_HEAP_FLAG_NONE,
        &texdesc12,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(pTexture.ReleaseAndGetAddressOf())
      )
    );

    m_texture = new D3DTexture2D(
      pTexture.Get(),
      TEXTURE_BIND_FLAG_SHADER_RESOURCE,
      dxgi_format,
      DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_UNKNOWN,
      DXGI_FORMAT_UNKNOWN,
      false,
      D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    // EXISTINGD3D11TODO: better debug names
    D3D::SetDebugObjectName12(m_texture->GetTex(), "a texture of the TextureCache");
  }
}

DXTexture::~DXTexture()
{
  m_texture->Release();
}

D3DTexture2D* DXTexture::GetRawTexIdentifier() const
{
  return m_texture;
}

void DXTexture::Bind(u32 stage)
{
  
}

bool DXTexture::Save(const std::string& filename, u32 level)
{
  u32 level_width = std::max(m_config.width >> level, 1u);
  u32 level_height = std::max(m_config.height >> level, 1u);
  size_t level_pitch = level_width;
  size_t num_lines = level_height;
  if (this->compressed)
  {
    level_pitch = (level_pitch + 3) >> 2;
    level_pitch *= 16; // Size of the bc2 block
    num_lines = (num_lines + 3) >> 2;
  }
  else
  {
    level_pitch *= sizeof(u32);
  }
  level_pitch = Common::AlignUpSizePow2(level_pitch, D3D12_TEXTURE_DATA_PITCH_ALIGNMENT);
  size_t required_readback_buffer_size = level_pitch * num_lines;
  ID3D12Resource* readback_buffer = nullptr;
  CD3DX12_HEAP_PROPERTIES hprop(D3D12_HEAP_TYPE_READBACK);
  auto rdesc = CD3DX12_RESOURCE_DESC::Buffer(required_readback_buffer_size);
  CheckHR(D3D::device->CreateCommittedResource(
    &hprop,
    D3D12_HEAP_FLAG_NONE,
    &rdesc,
    D3D12_RESOURCE_STATE_COPY_DEST,
    nullptr,
    IID_PPV_ARGS(&readback_buffer)));

  m_texture->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_COPY_SOURCE);

  D3D12_TEXTURE_COPY_LOCATION dst_location = {};
  dst_location.pResource = readback_buffer;
  dst_location.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
  dst_location.PlacedFootprint.Offset = 0;
  dst_location.PlacedFootprint.Footprint.Depth = 1;
  dst_location.PlacedFootprint.Footprint.Format = m_texture->GetFormat();
  dst_location.PlacedFootprint.Footprint.Width = level_width;
  dst_location.PlacedFootprint.Footprint.Height = level_height;
  dst_location.PlacedFootprint.Footprint.RowPitch = static_cast<UINT>(level_pitch);

  D3D12_TEXTURE_COPY_LOCATION src_location = CD3DX12_TEXTURE_COPY_LOCATION(m_texture->GetTex(), level);

  D3D::current_command_list->CopyTextureRegion(&dst_location, 0, 0, 0, &src_location, nullptr);

  D3D::command_list_mgr->ExecuteQueuedWork(true);

  // Map readback buffer and save to file.
  void* readback_texture_map;
  D3D12_RANGE read_range = { 0, required_readback_buffer_size };
  CheckHR(readback_buffer->Map(0, &read_range, &readback_texture_map));

  bool saved = false;
  if (this->compressed)
  {
    saved = TextureToDDS(
      static_cast<u8*>(readback_texture_map),
      dst_location.PlacedFootprint.Footprint.RowPitch,
      filename,
      dst_location.PlacedFootprint.Footprint.Width,
      dst_location.PlacedFootprint.Footprint.Height
    );
  }
  else
  {
    saved = TextureToPng(
      static_cast<u8*>(readback_texture_map),
      dst_location.PlacedFootprint.Footprint.RowPitch,
      filename,
      dst_location.PlacedFootprint.Footprint.Width,
      dst_location.PlacedFootprint.Footprint.Height
    );
  }
  m_texture->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
  D3D12_RANGE write_range = {};
  readback_buffer->Unmap(0, &write_range);
  readback_buffer->Release();
  return saved;
}

void DXTexture::CopyTexture(D3DTexture2D* source, D3DTexture2D* destination,
  u32 srcwidth, u32 srcheight,
  u32 dstwidth, u32 dstheight) 
{
  if (source->GetFormat() == destination->GetFormat()
    && srcwidth == dstwidth && srcheight == dstheight)
  {
    D3D::current_command_list->CopyResource(destination->GetTex(), source->GetTex());
    return;
  }
  D3D::SetViewportAndScissor(0, 0, dstwidth, dstheight);

  destination->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_RENDER_TARGET);
  auto rtv = destination->GetRTV();
  D3D::current_command_list->OMSetRenderTargets(1, &rtv, FALSE, nullptr);

  D3D::SetLinearCopySampler();
  D3D12_RECT srcRC;
  srcRC.left = 0;
  srcRC.right = srcwidth;
  srcRC.top = 0;
  srcRC.bottom = srcheight;
  D3D::DrawShadedTexQuad(source, &srcRC,
    srcwidth, srcheight,
    StaticShaderCache::GetColorCopyPixelShader(false),
    StaticShaderCache::GetSimpleVertexShader(),
    StaticShaderCache::GetSimpleVertexShaderInputLayout(), StaticShaderCache::GetCopyGeometryShader(), 0,
    destination->GetFormat(), false, destination->GetMultisampled());
  destination->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
  g_renderer->RestoreAPIState();
}

void DXTexture::CopyRectangle(D3DTexture2D* source, D3DTexture2D* destination,
  const MathUtil::Rectangle<int>& srcrect, u32 srcwidth, u32 srcheight,
  const MathUtil::Rectangle<int>& dstrect, u32 dstwidth, u32 dstheight)
{
  D3D::SetViewportAndScissor(dstrect.left, dstrect.top, dstrect.GetWidth(), dstrect.GetHeight());

  destination->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_RENDER_TARGET);
  auto rtv = destination->GetRTV();
  D3D::current_command_list->OMSetRenderTargets(1, &rtv, FALSE, nullptr);

  D3D::SetLinearCopySampler();
  D3D12_RECT srcRC;
  srcRC.left = srcrect.left;
  srcRC.right = srcrect.right;
  srcRC.top = srcrect.top;
  srcRC.bottom = srcrect.bottom;
  D3D::DrawShadedTexQuad(source, &srcRC,
   srcwidth, srcheight,
    StaticShaderCache::GetColorCopyPixelShader(false),
    StaticShaderCache::GetSimpleVertexShader(),
    StaticShaderCache::GetSimpleVertexShaderInputLayout(), StaticShaderCache::GetCopyGeometryShader(), 0,
    destination->GetFormat(), false, destination->GetMultisampled());
  destination->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
  g_renderer->RestoreAPIState();
}

void DXTexture::CopyRectangleFromTexture(const HostTexture* source,
  const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
  const DXTexture* srcentry = static_cast<const DXTexture*>(source);
  if (this->GetRawTexIdentifier()->GetFormat() == srcentry->GetRawTexIdentifier()->GetFormat()
    && srcrect.GetWidth() == dstrect.GetWidth()
    && srcrect.GetHeight() == dstrect.GetHeight())
  {
    CD3DX12_BOX src_box(srcrect.left, srcrect.top, 0, srcrect.right, srcrect.bottom, srcentry->GetConfig().layers);

    D3D12_TEXTURE_COPY_LOCATION dst = CD3DX12_TEXTURE_COPY_LOCATION(m_texture->GetTex(), 0);
    D3D12_TEXTURE_COPY_LOCATION src = CD3DX12_TEXTURE_COPY_LOCATION(srcentry->m_texture->GetTex(), 0);

    m_texture->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_COPY_DEST);
    srcentry->m_texture->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_COPY_SOURCE);

    D3D::current_command_list->CopyTextureRegion(&dst, dstrect.left, dstrect.top, 0, &src, &src_box);

    return;
  }
  else if (!m_config.rendertarget)
  {
    m_config.rendertarget = true;
    D3DTexture2D* ptexture = D3DTexture2D::Create(m_config.width, m_config.height,
      TEXTURE_BIND_FLAG_SHADER_RESOURCE | TEXTURE_BIND_FLAG_RENDER_TARGET,
      DXGI_FORMAT_R8G8B8A8_UNORM, m_config.levels, m_config.layers);
    ptexture->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_COPY_DEST);
    m_texture->TransitionToResourceState(D3D::current_command_list, D3D12_RESOURCE_STATE_COPY_SOURCE);
    CopyTexture(ptexture, m_texture, m_config.width, m_config.height, m_config.width, m_config.height);
    m_texture->Release();
    m_texture = ptexture;
  }
  CopyRectangle(srcentry->m_texture, m_texture, srcrect, srcentry->m_config.width, srcentry->m_config.height, dstrect, m_config.width, m_config.height);
}

void DXTexture::Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level)
{
  D3D::ReplaceTexture2D(m_texture->GetTex(), src, m_texture->GetFormat(), width, height, expanded_width, level, m_texture->GetResourceUsageState());
}
}  // namespace DX11
