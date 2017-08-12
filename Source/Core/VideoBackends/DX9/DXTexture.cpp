// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>
#include <cstddef>

#include "Common/Assert.h"
#include "Common/Align.h"
#include "Common/CommonTypes.h"
#include "Common/Logging/Log.h"

#include "VideoBackends/DX9/D3DBase.h"
#include "VideoBackends/DX9/D3DUtil.h"
#include "VideoBackends/DX9/D3DTexture.h"
#include "VideoBackends/DX9/DXTexture.h"
#include "VideoBackends/DX9/FramebufferManager.h"
#include "VideoBackends/DX9/PixelShaderCache.h"
#include "VideoBackends/DX9/Render.h"
#include "VideoBackends/DX9/VertexShaderCache.h"

#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/TextureConfig.h"
#include "VideoCommon/TextureCacheBase.h"

namespace DX9
{

static const D3DFORMAT HostTextureFormat_To_D3DFORMAT[]
{
  D3DFMT_UNKNOWN,//PC_TEX_FMT_NONE
  D3DFMT_A8R8G8B8,//PC_TEX_FMT_BGRA32
  D3DFMT_A8R8G8B8,//PC_TEX_FMT_RGBA32
  D3DFMT_A8P8,//PC_TEX_FMT_I4_AS_I8 A hack which means the format is a packed 8-bit intensity texture. It is unpacked to A8L8 in D3DTexture.cpp
  D3DFMT_A8L8,//PC_TEX_FMT_IA4_AS_IA8
  D3DFMT_A8P8,//PC_TEX_FMT_I8
  D3DFMT_A8L8,//PC_TEX_FMT_IA8
  D3DFMT_R5G6B5,//PC_TEX_FMT_RGB565
  D3DFMT_DXT1,//PC_TEX_FMT_DXT1
  D3DFMT_DXT3,//PC_TEX_FMT_DXT3
  D3DFMT_DXT5,//PC_TEX_FMT_DXT5
  D3DFMT_UNKNOWN,//PC_TEX_FMT_BPTC
  D3DFMT_D32F_LOCKABLE,//PC_TEX_FMT_DEPTH_FLOAT
  D3DFMT_R32F,//PC_TEX_FMT_R_FLOAT
  D3DFMT_A16B16G16R16F, //PC_TEX_FMT_RGBA16_FLOAT
  D3DFMT_A32B32G32R32F,//PC_TEX_FMT_RGBA_FLOAT
};

static const u32 HostTextureFormat_To_Buffer_Index[]
{
  0,//PC_TEX_FMT_NONE
  0,//PC_TEX_FMT_BGRA32
  0,//PC_TEX_FMT_RGBA32
  1,//PC_TEX_FMT_I4_AS_I8 A hack which means the format is a packed 8-bit intensity texture. It is unpacked to A8L8 in D3DTexture.cpp
  1,//PC_TEX_FMT_IA4_AS_IA8
  1,//PC_TEX_FMT_I8
  1,//PC_TEX_FMT_IA8
  2,//PC_TEX_FMT_RGB565
  3,//PC_TEX_FMT_DXT1
  4,//PC_TEX_FMT_DXT3
  5,//PC_TEX_FMT_DXT5
  0,//PC_TEX_FMT_BPTC
  6,//PC_TEX_FMT_DEPTH_FLOAT
  7,//PC_TEX_FMT_R_FLOAT
  8,//PC_TEX_FMT_RGBA16_FLOAT
  9,//PC_TEX_FMT_RGBA_FLOAT
};

#define MEM_TEXTURE_POOL_SIZE 10
static LPDIRECT3DTEXTURE9 s_memPoolTexture[MEM_TEXTURE_POOL_SIZE];
static u32 s_memPoolTextureW[MEM_TEXTURE_POOL_SIZE];
static u32 s_memPoolTextureH[MEM_TEXTURE_POOL_SIZE];

void DXTexture::Initialize()
{
  for (size_t i = 0; i < MEM_TEXTURE_POOL_SIZE; i++)
  {
    s_memPoolTexture[i] = nullptr;
    s_memPoolTextureW[i] = 1024u;
    s_memPoolTextureH[i] = 1024u;
  }
}
void DXTexture::Dispose()
{
  for (size_t i = 0; i < MEM_TEXTURE_POOL_SIZE; i++)
  {
    if (s_memPoolTexture[i])
    {
      s_memPoolTexture[i]->Release();
      s_memPoolTexture[i] = nullptr;
    }
  }
}

DXTexture::DXTexture(const TextureConfig& tex_config) : HostTexture(tex_config)
{
  d3d_fmt = HostTextureFormat_To_D3DFORMAT[m_config.pcformat];
  if (m_config.rendertarget)
  {
    D3D::dev->CreateTexture(m_config.width, m_config.height, 1, D3DUSAGE_RENDERTARGET,
      d3d_fmt, D3DPOOL_DEFAULT, &m_texture, 0);
    return;
  }
  m_texture = D3D::CreateTexture2D(m_config.width, m_config.height, d3d_fmt, m_config.levels);
  compressed = TexDecoder::IsCompressed(m_config.pcformat);
}

DXTexture::~DXTexture()
{
  m_texture->Release();
}

LPDIRECT3DTEXTURE9 DXTexture::GetRawTexIdentifier() const
{
  return m_texture;
}

void DXTexture::Bind(u32 stage)
{
  D3D::SetTexture(stage, m_texture);
}

bool DXTexture::Save(const std::string& filename, u32 level)
{
  IDirect3DSurface9* surface;
  HRESULT hr = m_texture->GetSurfaceLevel(level, &surface);
  if (FAILED(hr))
    return false;

  hr = PD3DXSaveSurfaceToFileA(filename.c_str(), this->compressed ? D3DXIFF_DDS : D3DXIFF_PNG, surface, NULL, NULL);
  surface->Release();

  return SUCCEEDED(hr);
}

void DXTexture::CopyRectangleFromTexture(const HostTexture* source,
  const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
  const DXTexture* entry = static_cast<const DXTexture*>(source);
  if (entry->d3d_fmt != d3d_fmt || d3d_fmt != D3DFMT_A8R8G8B8)
  {
    return;
  }
  LPDIRECT3DSURFACE9 srcsurf = nullptr;
  LPDIRECT3DSURFACE9 dstsurf = nullptr;
  if (!m_config.rendertarget)
  {
    m_config.rendertarget = true;
    LPDIRECT3DTEXTURE9 text;
    D3D::dev->CreateTexture(m_config.width, m_config.height, 1, D3DUSAGE_RENDERTARGET,
      D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &text, 0);
    m_texture->GetSurfaceLevel(0, &srcsurf);
    text->GetSurfaceLevel(0, &dstsurf);
    HRESULT hr = D3D::dev->StretchRect(srcsurf, nullptr, dstsurf, nullptr, D3DTEXF_LINEAR);
    _assert_msg_(VIDEO, SUCCEEDED(hr), "Failed updating texture");
    srcsurf->Release();
    m_texture->Release();
    m_texture = text;
    srcsurf = nullptr;
  }
  else
  {
    m_texture->GetSurfaceLevel(0, &dstsurf);
  }
  entry->m_texture->GetSurfaceLevel(0, &srcsurf);
  if (srcsurf != nullptr && dstsurf != nullptr)
  {
    HRESULT hr = D3D::dev->StretchRect(srcsurf, (RECT*)&srcrect, dstsurf, (RECT*)&dstrect, D3DTEXF_POINT);
    _assert_msg_(VIDEO, SUCCEEDED(hr), "Failed updating texture");
  }
  if (srcsurf != nullptr)
  {
    srcsurf->Release();
  }
  if (dstsurf != nullptr)
  {
    dstsurf->Release();
  }
}

void DXTexture::ReplaceTexture(const u8* src, u32 width, u32 height,
  u32 expanded_width, u32 level, bool swap_r_b)
{
  u32 pcformatidx = HostTextureFormat_To_Buffer_Index[m_config.pcformat];
  if (s_memPoolTexture[pcformatidx] == nullptr || width > s_memPoolTextureW[pcformatidx] || height > s_memPoolTextureH[pcformatidx])
  {
    if (s_memPoolTexture[pcformatidx] != nullptr)
    {
      s_memPoolTexture[pcformatidx]->Release();
      s_memPoolTexture[pcformatidx] = nullptr;
    }
    u32 max = std::max(width, height);
    u32 nextsize = s_memPoolTextureW[pcformatidx];
    while (nextsize < max)
    {
      nextsize *= 2;
    }
    s_memPoolTextureW[pcformatidx] = nextsize;
    s_memPoolTextureH[pcformatidx] = nextsize;
    s_memPoolTexture[pcformatidx] = D3D::CreateTexture2D(s_memPoolTextureW[pcformatidx], s_memPoolTextureH[pcformatidx], d3d_fmt, 1, D3DPOOL_SYSTEMMEM);
  }
  D3D::ReplaceTexture2D(s_memPoolTexture[pcformatidx], src, width, height, expanded_width, d3d_fmt, swap_r_b);
  PDIRECT3DSURFACE9 srcsurf;
  s_memPoolTexture[pcformatidx]->GetSurfaceLevel(0, &srcsurf);
  PDIRECT3DSURFACE9 dstsurface;
  m_texture->GetSurfaceLevel(level, &dstsurface);
  RECT srcr{ 0, 0, LONG(width), LONG(height) };
  POINT dstp{ 0, 0 };
  D3D::dev->UpdateSurface(srcsurf, &srcr, dstsurface, &dstp);
  srcsurf->Release();
  dstsurface->Release();
}

void DXTexture::Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level)
{
  bool swap_r_b = PC_TEX_FMT_RGBA32 == m_config.pcformat;
  ReplaceTexture(src, width, height, expanded_width, level, swap_r_b);
}
}  // namespace DX11
