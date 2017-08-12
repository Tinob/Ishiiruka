// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "Common/Assert.h"
#include "Common/CommonTypes.h"
#include "Common/GL/GLInterfaceBase.h"
#include "Common/MsgHandler.h"

#include "VideoBackends/OGL/FramebufferManager.h"
#include "VideoBackends/OGL/OGLTexture.h"
#include "VideoBackends/OGL/Render.h"
#include "VideoBackends/OGL/SamplerCache.h"
#include "VideoBackends/OGL/TextureCache.h"

#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/TextureConfig.h"

namespace OGL
{
namespace
{
std::array<u32, 16> s_Textures;
u32 s_ActiveTexture;
}  // Anonymous namespace

void OGLTexture::SetFormat()
{
  compressed = false;
  switch (m_config.pcformat)
  {
  case PC_TEX_FMT_BGRA32:
    gl_format = GL_BGRA;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_RGBA8;
    gl_type = GL_UNSIGNED_BYTE;
    break;

  case PC_TEX_FMT_RGBA32:
    gl_format = GL_RGBA;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_RGBA8;
    gl_type = GL_UNSIGNED_BYTE;
    break;
  case PC_TEX_FMT_I4_AS_I8:
    gl_format = GL_LUMINANCE;
    gl_iformat = GL_INTENSITY4;
    gl_siformat = GL_R8;
    gl_type = GL_UNSIGNED_BYTE;
    break;

  case PC_TEX_FMT_IA4_AS_IA8:
    gl_format = GL_LUMINANCE_ALPHA;
    gl_iformat = GL_LUMINANCE4_ALPHA4;
    gl_siformat = GL_RG8;
    gl_type = GL_UNSIGNED_BYTE;
    break;

  case PC_TEX_FMT_I8:
    gl_format = GL_LUMINANCE;
    gl_iformat = GL_INTENSITY8;
    gl_siformat = GL_R8;
    gl_type = GL_UNSIGNED_BYTE;
    break;

  case PC_TEX_FMT_IA8:
    gl_format = GL_LUMINANCE_ALPHA;
    gl_iformat = GL_LUMINANCE8_ALPHA8;
    gl_siformat = GL_RG8;
    gl_type = GL_UNSIGNED_BYTE;
    break;
  case PC_TEX_FMT_RGB565:
    gl_format = GL_RGB;
    gl_iformat = GL_RGB5;
    gl_siformat = GL_RGB5;
    gl_type = GL_UNSIGNED_SHORT_5_6_5;
    break;
  case PC_TEX_FMT_DXT1:
    gl_format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    gl_iformat = GL_RGB;
    gl_siformat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    gl_type = GL_UNSIGNED_BYTE;
    compressed = true;
    break;
  case PC_TEX_FMT_DXT3:
    gl_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    gl_type = GL_UNSIGNED_BYTE;
    compressed = true;
    break;
  case PC_TEX_FMT_DXT5:
    gl_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    gl_type = GL_UNSIGNED_BYTE;
    compressed = true;
    break;
  case PC_TEX_FMT_BPTC:
    gl_format = GL_COMPRESSED_RGBA_BPTC_UNORM;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_COMPRESSED_RGBA_BPTC_UNORM;
    gl_type = GL_UNSIGNED_BYTE;
    compressed = true;
    break;
  case PC_TEX_FMT_DEPTH_FLOAT:
    gl_format = GL_DEPTH_COMPONENT32F;
    gl_iformat = GL_DEPTH_COMPONENT;
    gl_siformat = GL_DEPTH_COMPONENT32;
    gl_type = GL_FLOAT;
    compressed = false;
    break;
  case PC_TEX_FMT_R_FLOAT:
    gl_format = GL_R32F;
    gl_iformat = GL_RED;
    gl_siformat = GL_R32F;
    gl_type = GL_FLOAT;
    compressed = false;
    break;
  case PC_TEX_FMT_RGBA16_FLOAT:
    gl_format = GL_RGBA16F;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_RGBA16F;
    gl_type = GL_FLOAT;
    compressed = false;
    break;
  case PC_TEX_FMT_RGBA_FLOAT:
    gl_format = GL_RGBA32F;
    gl_iformat = GL_RGBA;
    gl_siformat = GL_RGBA32F;
    gl_type = GL_FLOAT;
    compressed = false;
    break;
  default:  
    break;
  }
}

OGLTexture::OGLTexture(const TextureConfig& tex_config) : HostTexture(tex_config)
{
  glGenTextures(1, &m_texId);

  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, m_config.levels - 1);
  SetFormat();

  if (g_ogl_config.bSupportsTextureStorage)
  {
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_config.levels, gl_siformat, m_config.width, m_config.height,
      m_config.layers);
  }

  if (m_config.rendertarget)
  {
    if (!g_ogl_config.bSupportsTextureStorage)
    {
      for (u32 level = 0; level < m_config.levels; level++)
      {
        glTexImage3D(GL_TEXTURE_2D_ARRAY, level, gl_format,
          std::max(m_config.width >> level, 1u),
          std::max(m_config.height >> level, 1u),
          m_config.layers, 0, gl_iformat, gl_type, nullptr);
      }
    }
    glGenFramebuffers(1, &m_framebuffer);
    FramebufferManager::SetFramebuffer(m_framebuffer);
    FramebufferManager::FramebufferTexture(GL_FRAMEBUFFER, (gl_iformat == GL_DEPTH_COMPONENT) ? GL_DEPTH_ATTACHMENT : GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_ARRAY, m_texId, 0);
  }
  SetStage();
}

OGLTexture::~OGLTexture()
{
  if (m_texId)
  {
    for (auto& gtex : s_Textures)
      if (gtex == m_texId)
        gtex = 0;
    glDeleteTextures(1, &m_texId);
    m_texId = 0;
  }

  if (m_framebuffer)
  {
    glDeleteFramebuffers(1, &m_framebuffer);
    m_framebuffer = 0;
  }
}

GLuint OGLTexture::GetRawTexIdentifier() const
{
  return m_texId;
}

GLuint OGLTexture::GetFramebuffer() const
{
  return m_framebuffer;
}

void OGLTexture::Bind(u32 stage)
{
  if (s_Textures[stage] != m_texId)
  {
    if (s_ActiveTexture != stage)
    {
      glActiveTexture(GL_TEXTURE0 + stage);
      s_ActiveTexture = stage;
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
    s_Textures[stage] = m_texId;
  }
}

bool OGLTexture::Save(const std::string& filename, u32 level)
{
  if (GLInterface->GetMode() != GLInterfaceMode::MODE_OPENGL)
    return false;
  int width = std::max(m_config.width >> level, 1u);
  int height = std::max(m_config.height >> level, 1u);
  int size = compressed ? (((width + 3) >> 2) * ((height + 3) >> 2) * 16) : (width * height * 4);
  std::vector<u8> data(size);
  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);
  bool saved = false;
  if (compressed)
  {
    glGetCompressedTexImage(GL_TEXTURE_2D_ARRAY, level, data.data());
    saved = TextureToDDS(data.data(), width * 4, filename, width, height);
  }
  else
  {
    glGetTexImage(GL_TEXTURE_2D_ARRAY, level, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    saved = TextureToPng(data.data(), width * 4, filename, width, height);
  }
  OGLTexture::SetStage();
  return saved;
}

void OGLTexture::CopyRectangleFromTexture(const HostTexture* source,
  const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
  const OGLTexture* srcentry = static_cast<const OGLTexture*>(source);
  if (this->gl_format == srcentry->gl_format
    && srcrect.GetWidth() == dstrect.GetWidth() && srcrect.GetHeight() == dstrect.GetHeight() &&
    g_ogl_config.bSupportsCopySubImage)
  {
    glCopyImageSubData(srcentry->m_texId, GL_TEXTURE_2D_ARRAY, 0, srcrect.left, srcrect.top, 0,
      m_texId, GL_TEXTURE_2D_ARRAY, 0, dstrect.left, dstrect.top, 0,
      dstrect.GetWidth(), dstrect.GetHeight(), srcentry->m_config.layers);
    return;
  }
  else if (!m_framebuffer)
  {
    m_config.rendertarget = true;
    glGenFramebuffers(1, &m_framebuffer);
    FramebufferManager::SetFramebuffer(m_framebuffer);
    FramebufferManager::FramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_TEXTURE_2D_ARRAY, m_texId, 0);
  }
  g_renderer->ResetAPIState();
  FramebufferManager::SetFramebuffer(m_framebuffer);
  glActiveTexture(g_ActiveConfig.backend_info.bSupportsBindingLayout ? GL_TEXTURE9 : GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, srcentry->m_texId);
  g_sampler_cache->BindLinearSampler(g_ActiveConfig.backend_info.bSupportsBindingLayout ? 9 : 0);
  glViewport(dstrect.left, dstrect.top, dstrect.GetWidth(), dstrect.GetHeight());
  static_cast<OGL::TextureCache*>(g_texture_cache.get())->GetColorCopyProgram().Bind();
  glUniform4f(static_cast<OGL::TextureCache*>(g_texture_cache.get())->GetColorCopyPositionUniform(), float(srcrect.left),
    float(srcrect.top), float(srcrect.GetWidth()), float(srcrect.GetHeight()));
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  FramebufferManager::SetFramebuffer(0);
  g_renderer->RestoreAPIState();
}

void OGLTexture::Load(const u8* src, u32 width, u32 height,
  u32 expanded_width, u32 level)
{
  glActiveTexture(GL_TEXTURE9);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_texId);

  u32 blocksize = (m_config.pcformat == PC_TEX_FMT_DXT1) ? 8u : 16u;
  switch (m_config.pcformat)
  {
  case PC_TEX_FMT_DXT1:
  case PC_TEX_FMT_DXT3:
  case PC_TEX_FMT_DXT5:
  case PC_TEX_FMT_BPTC:
  {
    if (expanded_width != width)
    {
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_WIDTH, 4);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_HEIGHT, 4);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_DEPTH, 1);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_SIZE, blocksize);
      glPixelStorei(GL_UNPACK_ROW_LENGTH, expanded_width);
    }
    if (g_ogl_config.bSupportsTextureStorage)
    {
      glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, 0,
        width, height, 1, gl_format, ((width + 3) >> 2) * ((height + 3) >> 2) * blocksize, src);
    }
    else
    {
      glCompressedTexImage3D(GL_TEXTURE_2D_ARRAY, level, gl_format,
        width, height, 1, 0, ((width + 3) >> 2) * ((height + 3) >> 2) * blocksize, src);
    }
    if (expanded_width != width)
    {
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_WIDTH, 0);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_HEIGHT, 0);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_DEPTH, 0);
      glPixelStorei(GL_UNPACK_COMPRESSED_BLOCK_SIZE, 0);
    }
  }
  break;
  default:
  {
    if (expanded_width != width)
      glPixelStorei(GL_UNPACK_ROW_LENGTH, expanded_width);
    if (g_ogl_config.bSupportsTextureStorage)
    {
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, level, 0, 0, 0, width, height, 1, gl_format,
        gl_type, src);
    }
    else
    {
      glTexImage3D(GL_TEXTURE_2D_ARRAY, level, gl_iformat, width, height, 1, 0, gl_format,
        gl_type, src);
    }
    if (expanded_width != width)
      glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  }
  break;
  }
  SetStage();
}

void OGLTexture::DisableStage(u32 stage)
{
}

void OGLTexture::SetStage()
{
  // -1 is the initial value as we don't know which texture should be bound
  if (s_ActiveTexture != (u32)-1)
    glActiveTexture(GL_TEXTURE0 + s_ActiveTexture);
}

}  // namespace OGL
