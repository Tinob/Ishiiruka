// Copyright 2011 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <tuple>
#include <unordered_map>

#include "Common/GL/GLUtil.h"
#include "Common/LinearDiskCache.h"

#include "VideoCommon/GeometryShaderGen.h"
#include "VideoCommon/ObjectUsageProfiler.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/UberShaderCommon.h"
#include "VideoCommon/UberShaderPixel.h"
#include "VideoCommon/UberShaderVertex.h"
#include "VideoCommon/VertexShaderGen.h"



namespace OGL
{
class GLVertexFormat;
class SHADERUID
{
  size_t hash = {};
public:
  VertexShaderUid vuid;
  PixelShaderUid puid;
  GeometryShaderUid guid;
  void CalculateHash()
  {
    VertexShaderUid::ShaderUidHasher vshasher;
    PixelShaderUid::ShaderUidHasher pshasher;
    GeometryShaderUid::ShaderUidHasher gshasher;
    hash = vshasher(vuid) ^ pshasher(puid) ^ gshasher(guid);
  }
  bool operator <(const SHADERUID& r) const
  {
    return std::tie(vuid, puid, guid) < std::tie(r.vuid, r.puid, r.guid);
  }

  bool operator ==(const SHADERUID& r) const
  {
    return std::tie(vuid, puid, guid) == std::tie(r.vuid, r.puid, r.guid);
  }

  struct ShaderUidHasher
  {
    std::size_t operator()(const SHADERUID& k) const
    {
      return k.hash;
    }
  };
};

class UBERSHADERUID
{
  size_t hash = {};
public:
  UberShader::VertexUberShaderUid vuid;
  UberShader::PixelUberShaderUid puid;
  GeometryShaderUid guid;
  void CalculateHash()
  {
    UberShader::VertexUberShaderUid::ShaderUidHasher vshasher;
    UberShader::PixelUberShaderUid::ShaderUidHasher pshasher;
    GeometryShaderUid::ShaderUidHasher gshasher;
    hash = vshasher(vuid) ^ pshasher(puid) ^ gshasher(guid);
  }
  bool operator <(const UBERSHADERUID& r) const
  {
    return std::tie(vuid, puid, guid) < std::tie(r.vuid, r.puid, r.guid);
  }

  bool operator ==(const UBERSHADERUID& r) const
  {
    return std::tie(vuid, puid, guid) == std::tie(r.vuid, r.puid, r.guid);
  }

  struct ShaderUidHasher
  {
    std::size_t operator()(const UBERSHADERUID& k) const
    {
      return k.hash;
    }
  };
};


struct SHADER
{
  SHADER() : glprogid(0), initialized(false)
  {}
  void Destroy()
  {
    if (glprogid != 0)
    {
      glDeleteProgram(glprogid);
    }
    glprogid = 0;
    initialized = false;
  }
  GLuint glprogid = 0; // OpenGL program id
  bool initialized = false;
  void SetProgramVariables();
  void SetProgramBindings(bool is_compute);
  void Bind();
};

class ProgramShaderCache
{
public:

  static GLuint GetCurrentProgram();
  static SHADER* SetShader(PIXEL_SHADER_RENDER_MODE render_mode, u32 components, PrimitiveType primitive_type, const GLVertexFormat* vertex_format);
  static SHADER* SetUberShader(PrimitiveType primitive_type, u32 components, const GLVertexFormat* vertex_format);
  static void BindVertexFormat(const GLVertexFormat* vertex_format);
  static void InvalidateVertexFormat();
  static void BindLastVertexFormat();
  static SHADER* CompileShader(const SHADERUID& uid);
  static SHADER* CompileUberShader(const UBERSHADERUID& uid);
  static void GetShaderId(SHADERUID *uid, PIXEL_SHADER_RENDER_MODE render_mode, u32 components, PrimitiveType primitive_type);

  static bool CompileShader(SHADER &shader, const char* vcode, const char* pcode, const char* gcode = nullptr, const char **macros = nullptr, const u32 macro_count = 0);
  static bool CompileComputeShader(SHADER& shader, const std::string& code);
  static GLuint CompileSingleShader(GLuint type, const char *code, const char **macros = nullptr, const u32 count = 0);
  static void UploadConstants();

  static void Init();
  static void Shutdown(bool shadersonly = false);
  static void CreateHeader();
  static void Reload();
  static u32 GetUniformBufferAlignment();

private:
  struct PCacheEntry
  {
    SHADER shader;
    bool in_cache;

    void Destroy()
    {
      shader.Destroy();
    }
  };

  typedef ObjectUsageProfiler<SHADERUID, pKey_t, PCacheEntry, SHADERUID::ShaderUidHasher> PCache;
  typedef std::unordered_map<UBERSHADERUID, PCacheEntry, UBERSHADERUID::ShaderUidHasher> UberPCache;

  static void LoadFromDisk();
  static void CompileShaders();
  static void CompileUberShaders();

  class ProgramShaderCacheInserter : public LinearDiskCacheReader<SHADERUID, u8>
  {
  public:
    void Read(const SHADERUID &key, const u8 *value, u32 value_size) override;
  };

  class ProgramUberShaderCacheInserter : public LinearDiskCacheReader<UBERSHADERUID, u8>
  {
  public:
    void Read(const UBERSHADERUID &key, const u8 *value, u32 value_size) override;
  };

  static PCache* pshaders;
  static UberPCache pushaders;
  static std::array<PCacheEntry*, PIXEL_SHADER_RENDER_MODE::PSRM_DEPTH_ONLY + 1> last_entry;
  static std::array<SHADERUID, PIXEL_SHADER_RENDER_MODE::PSRM_DEPTH_ONLY + 1>  last_uid;

  static PCacheEntry* last_uber_entry;
  static UBERSHADERUID last_uber_uid;

  static u32 s_ubo_buffer_size;
  static u32 s_p_ubo_buffer_size;
  static u32 s_v_ubo_buffer_size;
  static u32 s_g_ubo_buffer_size;
  static s32 s_ubo_align;
  static u32 s_last_VAO;
};

}  // namespace OGL
