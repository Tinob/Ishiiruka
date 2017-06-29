// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include "VideoBackends/DX11/D3DPtr.h"
#include "VideoCommon/VideoCommon.h"

namespace DX11
{

class Television
{

public:

  Television();

  void Init();
  void Shutdown();

  // Submit video data to be drawn. This will change the current state of the
  // TV. xfbAddr points to YUYV data stored in GameCube/Wii RAM, but the XFB
  // may be virtualized when rendering so the RAM may not actually be read.
  void Submit(u32 xfbAddr, u32 stride, u32 width, u32 height);

  // Render the current state of the TV.
  void Render();

private:

  // Properties of last Submit call
  u32 m_curAddr;
  u32 m_curWidth;
  u32 m_curHeight;

  // Used for real XFB mode

  D3D::Texture2dPtr m_yuyvTexture;
  D3D::SrvPtr m_yuyvTextureSRV;
  D3D::PixelShaderPtr m_pShader;
  D3D::SamplerStatePtr m_samplerState;

};

}