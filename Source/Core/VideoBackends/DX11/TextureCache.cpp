// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Core/HW/Memmap.h"
#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DState.h"
#include "VideoBackends/DX11/D3DUtil.h"
#include "VideoBackends/DX11/DXTexture.h"
#include "VideoBackends/DX11/FramebufferManager.h"
#include "VideoBackends/DX11/PixelShaderCache.h"
#include "VideoBackends/DX11/CSTextureDecoder.h"
#include "VideoBackends/DX11/CSTextureEncoder.h"
#include "VideoBackends/DX11/PSTextureEncoder.h"
#include "VideoBackends/DX11/TextureCache.h"
#include "VideoBackends/DX11/TextureEncoder.h"
#include "VideoBackends/DX11/VertexShaderCache.h"
#include "VideoBackends/DX11/GeometryShaderCache.h"
#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/RenderBase.h"
#include "VideoCommon/VideoConfig.h"


namespace DX11
{

static std::unique_ptr<TextureEncoder> s_encoder;
static std::unique_ptr<TextureDecoder> s_decoder;
const size_t MAX_COPY_BUFFERS = 33;
D3D::BufferPtr efbcopycbuf[MAX_COPY_BUFFERS];

void TextureCache::LoadLut(u32 lutFmt, void* addr, u32 size)
{
  s_decoder->LoadLut(lutFmt, addr, size);
}

bool TextureCache::SupportsGPUTextureDecode(TextureFormat format, TlutFormat palette_format)
{
  return s_decoder->FormatSupported(format);
}

bool TextureCache::DecodeTextureOnGPU(HostTexture* dst, u32 dst_level, const u8* data,
  u32 data_size, TextureFormat tformat, u32 width, u32 height,
  u32 aligned_width, u32 aligned_height, u32 row_stride,
  const u8* palette, TlutFormat palette_format)
{
  return s_decoder->Decode(
    data,
    data_size,
    tformat,
    width,
    height,
    aligned_width,
    aligned_height,
    dst_level,
    *static_cast<DXTexture*>(dst)->GetRawTexIdentifier());
}

HostTextureFormat TextureCache::GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height)
{
  if (s_decoder->FormatSupported(texformat))
  {
    return PC_TEX_FMT_RGBA32;
  }
  const bool compressed_supported = ((width & 3) == 0) && ((height & 3) == 0);
  HostTextureFormat pcfmt = TexDecoder::GetHostTextureFormat(texformat, tlutfmt, compressed_supported);
  pcfmt = !g_ActiveConfig.backend_info.bSupportedFormats[pcfmt] ? PC_TEX_FMT_RGBA32 : pcfmt;
  return pcfmt;
}

std::unique_ptr<HostTexture> TextureCache::CreateTexture(const TextureConfig& config)
{
  return std::make_unique<DXTexture>(config);
}

void TextureCache::CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
  bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height)
{
  // When copying at half size, in multisampled mode, resolve the color/depth buffer first.
  // This is because multisampled texture reads go through Load, not Sample, and the linear
  // filter is ignored.
  bool multisampled = (g_ActiveConfig.iMultisamples > 1);
  ID3D11ShaderResourceView* efb_texture_srv;

  if (multisampled && scale_by_half)
  {
    multisampled = false;
    efb_texture_srv = is_depth_copy ?
      FramebufferManager::GetResolvedEFBDepthTexture()->GetSRV() :
      FramebufferManager::GetResolvedEFBColorTexture()->GetSRV();
  }
  else
  {
    efb_texture_srv = is_depth_copy ?
      FramebufferManager::GetEFBDepthTexture()->GetSRV() :
      FramebufferManager::GetEFBColorTexture()->GetSRV();
  }
  const TargetRectangle targetSource = g_renderer->ConvertEFBRectangle(src_rect);
  g_renderer->ResetAPIState();
  // stretch picture with increased internal resolution
  const D3D11_VIEWPORT vp = CD3D11_VIEWPORT(0.f, 0.f, (float)width, (float)height);
  D3D::context->RSSetViewports(1, &vp);

  // set transformation
  if (nullptr == efbcopycbuf[cbuf_id].get())
  {
    const D3D11_BUFFER_DESC cbdesc = CD3D11_BUFFER_DESC(28 * sizeof(float), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DEFAULT);
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = colmat;
    HRESULT hr = D3D::device->CreateBuffer(&cbdesc, &data, D3D::ToAddr(efbcopycbuf[cbuf_id]));
    CHECK(SUCCEEDED(hr), "Create efb copy constant buffer %d", cbuf_id);
    D3D::SetDebugObjectName(efbcopycbuf[cbuf_id].get(), "a constant buffer used in TextureCache::CopyRenderTargetToTexture");
  }
  D3D::stateman->SetPixelConstants(efbcopycbuf[cbuf_id].get());


  // TODO: try targetSource.asRECT();
  const D3D11_RECT sourcerect = CD3D11_RECT(targetSource.left, targetSource.top, targetSource.right, targetSource.bottom);

  // Use linear filtering if (bScaleByHalf), use point filtering otherwise
  if (scale_by_half)
    D3D::SetLinearCopySampler();
  else
    D3D::SetPointCopySampler();

  // if texture is currently in use, it needs to be temporarily unset
  u64 texture_mask = D3D::stateman->UnsetTexture(static_cast<DXTexture*>(entry->GetColor())->GetRawTexIdentifier()->GetSRV());
  D3D::stateman->Apply();
  D3D::context->OMSetRenderTargets(1, &static_cast<DXTexture*>(entry->GetColor())->GetRawTexIdentifier()->GetRTV(), nullptr);
  // Create texture copy
  D3D::drawShadedTexQuad(
    efb_texture_srv,
    &sourcerect, g_renderer->GetTargetWidth(), g_renderer->GetTargetHeight(),
    is_depth_copy ? PixelShaderCache::GetDepthMatrixProgram(multisampled) : PixelShaderCache::GetColorMatrixProgram(multisampled),
    VertexShaderCache::GetSimpleVertexShader(),
    VertexShaderCache::GetSimpleInputLayout(),
    (g_Config.iStereoMode > 0) ? GeometryShaderCache::GetCopyGeometryShader() : nullptr);

  D3D::context->OMSetRenderTargets(1, &FramebufferManager::GetEFBColorTexture()->GetRTV(), FramebufferManager::GetEFBDepthTexture()->GetDSV());

  g_renderer->RestoreAPIState();
  D3D::stateman->SetTextureByMask(texture_mask, static_cast<DXTexture*>(entry->GetColor())->GetRawTexIdentifier()->GetSRV());
}

void TextureCache::CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width,
  u32 bytes_per_row, u32 num_blocks_y, u32 memory_stride,
  bool is_depth_copy, const EFBRectangle& src_rect, bool scale_by_half)
{
  s_encoder->Encode(dst, format, native_width, bytes_per_row, num_blocks_y, memory_stride,
    is_depth_copy, src_rect, scale_by_half);
}

bool TextureCache::Palettize(TCacheEntry* entry, const TCacheEntry* base_entry)
{
  DX11::D3DTexture2D* srctexture = static_cast<DXTexture*>(base_entry->GetColor())->GetRawTexIdentifier();
  DX11::D3DTexture2D* texture = static_cast<DXTexture*>(entry->GetColor())->GetRawTexIdentifier();
  u32 texformat = entry->format & 0xf;
  BaseType baseType = Unorm4;
  if (texformat == GX_TF_C4 || texformat == GX_TF_I4)
    baseType = Unorm4;
  else if (texformat == GX_TF_C8 || texformat == GX_TF_I8)
    baseType = Unorm8;
  else
    return false;
  // if texture is currently in use, it needs to be temporarily unset
  u64 texture_mask = D3D::stateman->UnsetTexture(texture->GetSRV());
  D3D::stateman->Apply();
  bool result = s_decoder->Depalettize(*texture, *srctexture, baseType, base_entry->GetConfig().width, base_entry->GetConfig().height);
  D3D::stateman->SetTextureByMask(texture_mask, texture->GetSRV());
  return result;
}

TextureCache::TextureCache()
{
  if (g_ActiveConfig.backend_info.bSupportsComputeTextureEncoding
    && g_ActiveConfig.bEnableComputeTextureEncoding)
  {
    s_encoder = std::make_unique<CSTextureEncoder>();
  }
  else
  {
    s_encoder = std::make_unique<PSTextureEncoder>();
  }
  s_encoder->Init();
  s_decoder = std::make_unique<CSTextureDecoder>();
  s_decoder->Init();
}

TextureCache::~TextureCache()
{
  for (u32 k = 0; k < MAX_COPY_BUFFERS; ++k)
    efbcopycbuf[k].reset();

  s_encoder->Shutdown();
  s_encoder.reset();

  s_decoder->Shutdown();
  s_decoder.reset();
}

}
