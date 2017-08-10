// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <d3d11_2.h>

namespace DX11
{

namespace D3D
{
void ReplaceTexture2D(ID3D11Texture2D* pTexture, const u8* buffer, u32 width, u32 height, u32 pitch, u32 level, D3D11_USAGE usage, DXGI_FORMAT fmtconvert_rgb565, bool swap_rb, bool convert_rgb565);
}

class D3DTexture2D
{
public:
  // there are two ways to create a D3DTexture2D object:
  //     either create an ID3D11Texture2D object, pass it to the constructor and specify what views to create
  //     or let the texture automatically be created by D3DTexture2D::Create

  D3DTexture2D(
    ID3D11Texture2D* texptr,
    D3D11_BIND_FLAG bind,
    DXGI_FORMAT fmt,
    DXGI_FORMAT srv_format = DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT dsv_format = DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT rtv_format = DXGI_FORMAT_UNKNOWN,
    bool multisampled = false);
  static D3DTexture2D* Create(
    u32 width,
    u32 height,
    D3D11_BIND_FLAG bind,
    D3D11_USAGE usage,
    DXGI_FORMAT fmt,
    u32 levels = 1,
    u32 slices = 1,
    D3D11_SUBRESOURCE_DATA* data = nullptr);

  // reference counting, use AddRef() when creating a new reference and Release() it when you don't need it anymore
  void AddRef();
  UINT Release();

  ID3D11Texture2D* &GetTex()
  {
    return m_tex;
  }
  ID3D11ShaderResourceView* &GetSRV()
  {
    return m_srv;
  }
  ID3D11RenderTargetView* &GetRTV()
  {
    return m_rtv;
  }
  ID3D11DepthStencilView* &GetDSV()
  {
    return m_dsv;
  }
  ID3D11UnorderedAccessView* &GetUAV()
  {
    return m_uav;
  }
  DXGI_FORMAT GetFormat() const
  {
    return m_format;
  }
private:
  ~D3DTexture2D();

  ID3D11Texture2D* m_tex = {};
  ID3D11ShaderResourceView* m_srv = {};
  ID3D11RenderTargetView* m_rtv = {};
  ID3D11DepthStencilView* m_dsv = {};
  ID3D11UnorderedAccessView* m_uav = {};
  D3D11_BIND_FLAG m_bindflags = {};
  UINT m_ref = {};
  DXGI_FORMAT m_format = {};
};

}  // namespace DX11