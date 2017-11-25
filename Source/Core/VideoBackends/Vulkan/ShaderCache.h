// Copyright 2016 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "Common/CommonTypes.h"
#include "Common/LinearDiskCache.h"

#include "VideoBackends/Vulkan/Constants.h"
#include "VideoBackends/Vulkan/ObjectCache.h"
#include "VideoBackends/Vulkan/ShaderCompiler.h"

#include "VideoCommon/GeometryShaderGen.h"
#include "VideoCommon/ObjectUsageProfiler.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/RenderState.h"
#include "VideoCommon/UberShaderPixel.h"
#include "VideoCommon/UberShaderVertex.h"
#include "VideoCommon/VertexShaderGen.h"

namespace Vulkan
{
class CommandBufferManager;
class VertexFormat;
class StreamBuffer;

struct PipelineInfo
{
  // These are packed in descending order of size, to avoid any padding so that the structure
  // can be copied/compared as a single block of memory. 64-bit pointer size is assumed.
  const VertexFormat* vertex_format;
  VkPipelineLayout pipeline_layout;
  VkShaderModule vs;
  VkShaderModule gs;
  VkShaderModule ps;
  VkRenderPass render_pass;
  BlendingState blend_state;
  RasterizationState rasterization_state;
  DepthState depth_state;
  MultisamplingState multisampling_state;

  bool operator==(const PipelineInfo& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(rhs)) == 0;
  }

  bool operator!=(const PipelineInfo& rhs) const
  {
    return !operator==(rhs);
  }

  bool operator<(const PipelineInfo& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(rhs)) < 0;
  }

  bool operator>(const PipelineInfo& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(rhs)) > 0;
  }
};

struct PipelineInfoHash
{
  size_t operator()(const PipelineInfo& key) const
  {
    size_t h = -1;
    h = h * 137 + (uintptr_t)key.vertex_format;
    h = h * 137 + (uintptr_t)key.pipeline_layout;
    h = h * 137 + (uintptr_t)key.vs;
    h = h * 137 + (uintptr_t)key.gs;
    h = h * 137 + (uintptr_t)key.ps;
    h = h * 137 + (uintptr_t)key.render_pass;
    h = h * 137 + (uintptr_t)key.multisampling_state.hex;
    h = h * 137 + (uintptr_t)(((uintptr_t)key.blend_state.hex << 32) |
      key.rasterization_state.hex | (uintptr_t(key.depth_state.hex) << 12));
    return h;
  }
};

struct ComputePipelineInfo
{
  VkPipelineLayout pipeline_layout;
  VkShaderModule cs;
  bool operator==(const ComputePipelineInfo& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(rhs)) == 0;
  }

  bool operator!=(const ComputePipelineInfo& rhs) const
  {
    return !operator==(rhs);
  }

  bool operator<(const ComputePipelineInfo& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(rhs)) < 0;
  }

  bool operator>(const ComputePipelineInfo& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(rhs)) > 0;
  }
};

struct ComputePipelineInfoHash
{
  std::size_t operator()(const ComputePipelineInfo& key) const
  {
    size_t h = -1;
    h = h * 137 + (uintptr_t)key.pipeline_layout;
    h = h * 137 + (uintptr_t)key.cs;
    return h;
  }
};

class ShaderCache
{
public:
  ShaderCache();
  ~ShaderCache();

  // Get utility shader header based on current config.
  std::string GetUtilityShaderHeader() const;

  // Accesses ShaderGen shader caches
  VkShaderModule GetVertexShaderForUid(const VertexShaderUid& uid);
  VkShaderModule GetGeometryShaderForUid(const GeometryShaderUid& uid);
  VkShaderModule GetPixelShaderForUid(const PixelShaderUid& uid);

  // Ubershader caches
  VkShaderModule GetVertexUberShaderForUid(const UberShader::VertexUberShaderUid& uid);
  VkShaderModule GetPixelUberShaderForUid(const UberShader::PixelUberShaderUid& uid);

  // Perform at startup, create descriptor layouts, compiles all static shaders.
  bool Initialize();
  void Shutdown();

  // Creates a pipeline for the specified description. The resulting pipeline, if successful
  // is not stored anywhere, this is left up to the caller.
  VkPipeline CreatePipeline(const PipelineInfo& info);

  // Find a pipeline by the specified description, if not found, attempts to create it.
  VkPipeline GetPipeline(const PipelineInfo& info);

  // Find a pipeline by the specified description, if not found, attempts to create it. If this
  // resulted in a pipeline being created, the second field of the return value will be false,
  // otherwise for a cache hit it will be true.
  std::pair<VkPipeline, bool> GetPipelineWithCacheResult(const PipelineInfo& info);
  
  // Creates a compute pipeline, and does not track the handle.
  VkPipeline CreateComputePipeline(const ComputePipelineInfo& info);

  // Find a pipeline by the specified description, if not found, attempts to create it
  VkPipeline GetComputePipeline(const ComputePipelineInfo& info);

  // Clears our pipeline cache of all objects. This is necessary when recompiling shaders,
  // as drivers are free to return the same pointer again, which means that we may end up using
  // and old pipeline object if they are not cleared first. Some stutter may be experienced
  // while our cache is rebuilt on use, but the pipeline cache object should mitigate this.
  // NOTE: Ensure that none of these objects are in use before calling.
  void ClearPipelineCache();

  // Saves the pipeline cache to disk. Call when shutting down.
  void SavePipelineCache();

  // Recompile shared shaders, call when stereo mode changes.
  void RecompileSharedShaders();

  // Reload all shaders and pipelines
  void Reload();

  // Shared shader accessors
  VkShaderModule GetScreenQuadVertexShader() const { return m_screen_quad_vertex_shader; }
  VkShaderModule GetPassthroughVertexShader() const { return m_passthrough_vertex_shader; }
  VkShaderModule GetScreenQuadGeometryShader() const { return m_screen_quad_geometry_shader; }
  VkShaderModule GetPassthroughGeometryShader() const { return m_passthrough_geometry_shader; }

  class vkShaderItem
  {
  public:
    bool compiled{};
    std::atomic_flag initialized{};
    VkShaderModule module = VK_NULL_HANDLE;
    vkShaderItem() {}
  };

private:
  void CompileShaders();
  void CompileUberShaders();
  bool CreatePipelineCache(bool load_from_disk);
  bool ValidatePipelineCache(const u8* data, size_t data_length);
  void DestroyPipelineCache();
  void LoadShaderCaches(bool forcecompile = false);
  void DestroyShaderCaches();
  bool CompileSharedShaders();
  void DestroySharedShaders();

  

  template <typename Uid, typename UidHasher>
  class ShaderUsageModuleCache
  {
  public:
    typedef ObjectUsageProfiler<Uid, pKey_t, vkShaderItem, UidHasher> cache_type;
    std::unique_ptr<cache_type> shader_map{};
    LinearDiskCache<Uid, u32> disk_cache{};
    ShaderUsageModuleCache() {}
  };

  template <typename Uid, typename UidHasher>
  class ShaderModuleCache
  {
  public:
    typedef std::unordered_map<Uid, vkShaderItem, UidHasher> cache_type;
    cache_type shader_map{};
    LinearDiskCache<Uid, u32> disk_cache{};
    ShaderModuleCache() {}
  };

  using VShaderCache = ShaderUsageModuleCache<VertexShaderUid, VertexShaderUid::ShaderUidHasher> ;
  using PShaderCache = ShaderUsageModuleCache<PixelShaderUid, PixelShaderUid::ShaderUidHasher>;
  using GShaderCache = ShaderModuleCache<GeometryShaderUid, GeometryShaderUid::ShaderUidHasher>;
  using VUShaderCache = ShaderModuleCache<UberShader::VertexUberShaderUid, UberShader::VertexUberShaderUid::ShaderUidHasher>;
  using PUShaderCache = ShaderModuleCache<UberShader::PixelUberShaderUid, UberShader::PixelUberShaderUid::ShaderUidHasher>;


  VShaderCache m_vs_cache;
  GShaderCache m_gs_cache;
  PShaderCache m_ps_cache;
  VUShaderCache m_vus_cache;
  PUShaderCache m_pus_cache;

  void CompileVertexShaderForUid(const VertexShaderUid& uid, vkShaderItem& it);
  void CompileGeometryShaderForUid(const GeometryShaderUid& uid, vkShaderItem& it);
  void CompilePixelShaderForUid(const PixelShaderUid& uid, vkShaderItem& it);
  void CompileVertexUberShaderForUid(const UberShader::VertexUberShaderUid& uid, vkShaderItem& it);
  void CompilePixelUberShaderForUid(const UberShader::PixelUberShaderUid& uid, vkShaderItem& it);
  

  std::unordered_map<PipelineInfo, std::pair<VkPipeline, bool>, PipelineInfoHash>
      m_pipeline_objects;
  std::unordered_map<ComputePipelineInfo, VkPipeline, ComputePipelineInfoHash>
      m_compute_pipeline_objects;
  VkPipelineCache m_pipeline_cache = VK_NULL_HANDLE;
  std::string m_pipeline_cache_filename;

  // Utility/shared shaders
  VkShaderModule m_screen_quad_vertex_shader = VK_NULL_HANDLE;
  VkShaderModule m_passthrough_vertex_shader = VK_NULL_HANDLE;
  VkShaderModule m_screen_quad_geometry_shader = VK_NULL_HANDLE;
  VkShaderModule m_passthrough_geometry_shader = VK_NULL_HANDLE;
};

extern std::unique_ptr<ShaderCache> g_shader_cache;

}  // namespace Vulkan
