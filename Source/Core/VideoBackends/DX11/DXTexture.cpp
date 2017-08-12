// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>
#include <cstddef>

#include "Common/Assert.h"
#include "Common/Align.h"
#include "Common/CommonTypes.h"
#include "Common/Logging/Log.h"

#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DState.h"
#include "VideoBackends/DX11/D3DUtil.h"
#include "VideoBackends/DX11/D3DTexture.h"
#include "VideoBackends/DX11/DXTexture.h"
#include "VideoBackends/DX11/FramebufferManager.h"
#include "VideoBackends/DX11/PixelShaderCache.h"
#include "VideoBackends/DX11/Render.h"
#include "VideoBackends/DX11/VertexShaderCache.h"
#include "VideoBackends/DX11/GeometryShaderCache.h"

#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/TextureConfig.h"

namespace DX11
{
namespace
{
DXGI_FORMAT GetDXGIFormatForHostFormat(HostTextureFormat format)
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

  return HostTextureFormat_To_DXGIFORMAT[format];
}
}  // Anonymous namespace

DXTexture::DXTexture(const TextureConfig& tex_config) : HostTexture(tex_config)
{
  DXGI_FORMAT format = GetDXGIFormatForHostFormat(m_config.pcformat);
  if (m_config.rendertarget)
  {
    int flags = ((int)D3D11_BIND_RENDER_TARGET | (int)D3D11_BIND_SHADER_RESOURCE);
    if (D3D::GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
    {
      flags |= D3D11_BIND_UNORDERED_ACCESS;
    }
    m_texture = D3DTexture2D::Create(m_config.width, m_config.height,
      (D3D11_BIND_FLAG)flags,
      D3D11_USAGE_DEFAULT, format, 1, m_config.layers);
    return;
  }
  bool bgrasupported = D3D::BGRATexturesSupported();
  if (format == DXGI_FORMAT_B8G8R8A8_UNORM && !bgrasupported)
  {
    swap_rg = true;
    format = DXGI_FORMAT_R8G8B8A8_UNORM;
  }
  if (format == DXGI_FORMAT_B5G6R5_UNORM && !D3D::BGRA565TexturesSupported())
  {
    convertrgb565 = true;
    format = bgrasupported ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
  }
  compressed = TexDecoder::IsCompressed(m_config.pcformat);
  usage = D3D11_USAGE_DEFAULT;
  D3D11_CPU_ACCESS_FLAG cpu_access = (D3D11_CPU_ACCESS_FLAG)0;

  if (format == DXGI_FORMAT_B5G6R5_UNORM)
  {
    usage = D3D11_USAGE_DYNAMIC;
    cpu_access = D3D11_CPU_ACCESS_WRITE;
  }
  const D3D11_TEXTURE2D_DESC texdesc = CD3D11_TEXTURE2D_DESC(format,
    m_config.width, m_config.height, 1, m_config.levels, D3D11_BIND_SHADER_RESOURCE, usage, cpu_access);

  ID3D11Texture2D *pTexture;
  HRESULT hr = D3D::device->CreateTexture2D(&texdesc, NULL, &pTexture);
  CHECK(SUCCEEDED(hr), "Create texture of the TextureCache");
  m_texture = new D3DTexture2D(pTexture, D3D11_BIND_SHADER_RESOURCE, format);
  // TODO: better debug names
  D3D::SetDebugObjectName(m_texture->GetTex(), "a texture of the TextureCache");
  D3D::SetDebugObjectName(m_texture->GetSRV(), "shader resource view of a texture of the TextureCache");
  SAFE_RELEASE(pTexture);
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
  D3D::stateman->SetTexture(stage, m_texture->GetSRV());
}

bool DXTexture::Save(const std::string& filename, u32 level)
{
  // Create a staging/readback texture with the dimensions of the specified mip level.
  u32 mip_width = std::max(m_config.width >> level, 1u);
  u32 mip_height = std::max(m_config.height >> level, 1u);
  CD3D11_TEXTURE2D_DESC staging_texture_desc(
    m_texture->GetFormat(),
    mip_width, mip_height, 1, 1, 0,
    D3D11_USAGE_STAGING,
    D3D11_CPU_ACCESS_READ);

  ID3D11Texture2D* staging_texture;
  HRESULT hr = D3D::device->CreateTexture2D(&staging_texture_desc, nullptr, &staging_texture);
  if (FAILED(hr))
  {
    WARN_LOG(VIDEO, "Failed to create texture dumping readback texture: %X", static_cast<u32>(hr));
    return false;
  }

  // Copy the selected mip level to the staging texture.
  CD3D11_BOX src_box(0, 0, 0, mip_width, mip_height, 1);
  D3D::context->CopySubresourceRegion(staging_texture, 0, 0, 0, 0,
    m_texture->GetTex(), D3D11CalcSubresource(level, 0, m_config.levels), &src_box);

  // Map the staging texture to client memory, and encode it as a .png image.
  D3D11_MAPPED_SUBRESOURCE map;
  hr = D3D::context->Map(staging_texture, 0, D3D11_MAP_READ, 0, &map);
  if (FAILED(hr))
  {
    WARN_LOG(VIDEO, "Failed to map texture dumping readback texture: %X", static_cast<u32>(hr));
    staging_texture->Release();
    return false;
  }

  bool encode_result = false;
  if (this->compressed)
  {
    encode_result = TextureToDDS(reinterpret_cast<u8*>(map.pData), map.RowPitch, filename, mip_width, mip_height);
  }
  else
  {
    encode_result = TextureToPng(reinterpret_cast<u8*>(map.pData), map.RowPitch, filename, mip_width, mip_height);
  }
  D3D::context->Unmap(staging_texture, 0);
  staging_texture->Release();

  return encode_result;
}

void DXTexture::CopyTexture(D3DTexture2D* source, D3DTexture2D* destination,
  u32 srcwidth, u32 srcheight,
  u32 dstwidth, u32 dstheight)
{
  if (source->GetFormat() == destination->GetFormat()
    && srcwidth == dstwidth && srcheight == dstheight)
  {
    D3D::context->CopyResource(
      destination->GetTex(),
      source->GetTex());
    return;
  }
  g_renderer->ResetAPIState(); // reset any game specific settings
  const D3D11_VIEWPORT vp = CD3D11_VIEWPORT(
    float(0),
    float(0),
    float(dstwidth),
    float(dstheight));
  u64 texture_mask = D3D::stateman->UnsetTexture(destination->GetSRV());
  D3D::stateman->Apply();
  D3D::context->OMSetRenderTargets(1, &destination->GetRTV(), nullptr);
  D3D::context->RSSetViewports(1, &vp);
  D3D::SetLinearCopySampler();
  D3D11_RECT srcRC;
  srcRC.left = 0;
  srcRC.right = srcwidth;
  srcRC.top = 0;
  srcRC.bottom = srcheight;
  D3D::drawShadedTexQuad(destination->GetSRV(), &srcRC,
    srcwidth, srcheight,
    PixelShaderCache::GetColorCopyProgram(false),
    VertexShaderCache::GetSimpleVertexShader(),
    VertexShaderCache::GetSimpleInputLayout(), GeometryShaderCache::GetCopyGeometryShader(), 1.0, 0);

  D3D::context->OMSetRenderTargets(1,
    &FramebufferManager::GetEFBColorTexture()->GetRTV(),
    FramebufferManager::GetEFBDepthTexture()->GetDSV());

  g_renderer->RestoreAPIState();
  D3D::stateman->SetTextureByMask(texture_mask, destination->GetSRV());
}

void DXTexture::CopyRectangle(D3DTexture2D* source, D3DTexture2D* destination,
  const MathUtil::Rectangle<int>& srcrect, u32 srcwidth, u32 srcheight,
  const MathUtil::Rectangle<int>& dstrect, u32 dstwidth, u32 dstheight)
{
  g_renderer->ResetAPIState(); // reset any game specific settings

  const D3D11_VIEWPORT vp = CD3D11_VIEWPORT(
    float(dstrect.left),
    float(dstrect.top),
    float(dstrect.GetWidth()),
    float(dstrect.GetHeight()));
  u64 texture_mask = D3D::stateman->UnsetTexture(destination->GetSRV());
  D3D::stateman->Apply();
  D3D::context->OMSetRenderTargets(1, &destination->GetRTV(), nullptr);
  D3D::context->RSSetViewports(1, &vp);
  D3D::SetLinearCopySampler();
  D3D11_RECT srcRC;
  srcRC.left = srcrect.left;
  srcRC.right = srcrect.right;
  srcRC.top = srcrect.top;
  srcRC.bottom = srcrect.bottom;
  D3D::drawShadedTexQuad(destination->GetSRV(), &srcRC,
    srcwidth, srcheight,
    PixelShaderCache::GetColorCopyProgram(false),
    VertexShaderCache::GetSimpleVertexShader(),
    VertexShaderCache::GetSimpleInputLayout(), GeometryShaderCache::GetCopyGeometryShader(), 1.0, 0);

  D3D::context->OMSetRenderTargets(1,
    &FramebufferManager::GetEFBColorTexture()->GetRTV(),
    FramebufferManager::GetEFBDepthTexture()->GetDSV());

  g_renderer->RestoreAPIState();
  D3D::stateman->SetTextureByMask(texture_mask, destination->GetSRV());
}

void DXTexture::CopyRectangleFromTexture(const HostTexture* source,
  const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
  const DXTexture* srcentry = static_cast<const DXTexture*>(source);
  if (this->GetRawTexIdentifier()->GetFormat() == srcentry->GetRawTexIdentifier()->GetFormat()
    && srcrect.GetWidth() == dstrect.GetWidth()
    && srcrect.GetHeight() == dstrect.GetHeight()
    && static_cast<UINT>(dstrect.GetWidth()) <= m_config.width
    && static_cast<UINT>(dstrect.GetHeight()) <= m_config.height
    && static_cast<UINT>(dstrect.GetWidth()) <= source->GetConfig().width
    && static_cast<UINT>(dstrect.GetHeight()) <= source->GetConfig().height)
  {
    CD3D11_BOX src_box(srcrect.left, srcrect.top, 0, srcrect.right, srcrect.bottom, source->GetConfig().layers);
    D3D::context->CopySubresourceRegion(
      m_texture->GetTex(),
      0,
      dstrect.left,
      dstrect.top,
      0,
      srcentry->m_texture->GetTex(),
      0,
      &src_box);
    return;
  }
  else if (!m_config.rendertarget)
  {
    m_config.rendertarget = true;
    int flags = ((int)D3D11_BIND_RENDER_TARGET | (int)D3D11_BIND_SHADER_RESOURCE);
    if (D3D::GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
    {
      flags |= D3D11_BIND_UNORDERED_ACCESS;
    }
    D3DTexture2D* ptexture = D3DTexture2D::Create(m_config.width, m_config.height,
      (D3D11_BIND_FLAG)flags,
      D3D11_USAGE_DEFAULT, DXGI_FORMAT_R8G8B8A8_UNORM, 1, m_config.layers);

    CopyTexture(ptexture, m_texture, m_config.width, m_config.height, m_config.width, m_config.height);

    m_texture->Release();
    m_texture = ptexture;
  }
  CopyRectangle(srcentry->m_texture, m_texture, srcrect, srcentry->m_config.width, srcentry->m_config.height, dstrect, m_config.width, m_config.height);
}

void DXTexture::Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level)
{
  D3D::ReplaceTexture2D(
    m_texture->GetTex(),
    src,
    width,
    height,
    expanded_width,
    level,
    usage,
    m_texture->GetFormat(),
    swap_rg,
    convertrgb565);
}
}  // namespace DX11
