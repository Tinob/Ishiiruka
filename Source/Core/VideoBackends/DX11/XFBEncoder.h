// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include "VideoBackends/DX11/D3DPtr.h"
#include "VideoCommon/VideoCommon.h"

namespace DX11
{

class XFBEncoder
{

public:

  XFBEncoder();

  void Init();
  void Shutdown();

  void Encode(u8* dst, u32 width, u32 height, const EFBRectangle& srcRect, float gamma);

private:

  D3D::Texture2dPtr m_out;
  D3D::RtvPtr m_outRTV;
  D3D::Texture2dPtr m_outStage;
  D3D::BufferPtr m_encodeParams;
  D3D::BufferPtr m_quad;
  D3D::VertexShaderPtr m_vShader;
  D3D::InputLayoutPtr m_quadLayout;
  D3D::PixelShaderPtr m_pShader;
  D3D::BlendStatePtr m_xfbEncodeBlendState;
  D3D::DepthStencilStatePtr m_xfbEncodeDepthState;
  D3D::RasterizerStatePtr m_xfbEncodeRastState;
  D3D::SamplerStatePtr m_efbSampler;

};

}