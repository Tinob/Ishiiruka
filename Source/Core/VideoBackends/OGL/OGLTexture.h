#pragma once
// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include "Common/CommonTypes.h"
#include "Common/GL/GLUtil.h"

#include "VideoCommon/HostTexture.h"

namespace OGL
{
class OGLTexture final : public HostTexture
{
public:
  explicit OGLTexture(const TextureConfig& tex_config);
  ~OGLTexture();

  void Bind(u32 stage) override;
  bool Save(const std::string& filename, u32 level) override;

  void CopyRectangleFromTexture(const HostTexture* source,
    const MathUtil::Rectangle<int>& srcrect,
    const MathUtil::Rectangle<int>& dstrect) override;
  void Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level) override;

  GLuint GetRawTexIdentifier() const;
  GLuint GetFramebuffer() const;
  uintptr_t GetInternalObject() const override { return static_cast<uintptr_t>(m_texId); };
  static void DisableStage(u32 stage);
  static void SetStage();

private:
  void SetFormat();
  GLuint m_texId;
  GLuint m_framebuffer = 0;
  bool compressed = false;
  GLuint gl_format;
  GLuint gl_iformat;
  GLuint gl_siformat;
  GLuint gl_type;
};

}  // namespace OGL
