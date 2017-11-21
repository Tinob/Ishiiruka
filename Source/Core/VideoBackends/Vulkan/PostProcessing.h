// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include "Common/CommonTypes.h"
#include "VideoBackends/Vulkan/ShaderCompiler.h"
#include "VideoBackends/Vulkan/StreamBuffer.h"
#include "VideoCommon/PostProcessing.h"
#include "VideoCommon/VideoCommon.h"

namespace Vulkan
{
// PostProcessingShader comprises of all the resources needed to render a shader, including
// temporary buffers, external images, shader programs, and configurations.
class VulkanPostProcessingShader final : public PostProcessingShader
{
public:
  VulkanPostProcessingShader();
  ~VulkanPostProcessingShader();

  void MapAndUpdateConfigurationBuffer() override;
  void Draw(PostProcessor* parent,
    const TargetRectangle& dst_rect, const TargetSize& dst_size, uintptr_t dst_texture,
    const TargetRectangle& src_rect, const TargetSize& src_size, uintptr_t src_texture,
    uintptr_t src_depth_texture, int src_layer, float gamma) override;

private:
  struct RenderPassVulkanData final
  {
    VkShaderModule m_fragment_shader = VK_NULL_HANDLE;
  };
  void ReleaseBindingSampler(uintptr_t binding) override;
  uintptr_t CreateBindingSampler(const PostProcessingShaderConfiguration::RenderPass::Input& input_config) override;
  void ReleasePassNativeResources(RenderPassData& pass) override;
  bool RecompileShaders() override;
  std::vector<u8> m_constants;
};

class VulkanPostProcessor final : public PostProcessor
{
public:
  VulkanPostProcessor() : PostProcessor(API_VULKAN) {};
  ~VulkanPostProcessor();

  bool Initialize() override;

  void PostProcessEFB(const TargetRectangle& target_rect, const TargetSize& target_size) override;

  void PostProcessEFBToTexture(uintptr_t dst_texture) override;

  void MapAndUpdateUniformBuffer(
    const InputTextureSizeArray& input_sizes,
    const TargetRectangle& dst_rect, const TargetSize& dst_size,
    const TargetRectangle& src_rect, const TargetSize& src_size,
    int src_layer, float gamma);

  // NOTE: Can change current render target and viewport.
  // If src_layer <0, copy all layers, otherwise, copy src_layer to layer 0.
  void CopyTexture(const TargetRectangle& dst_rect, uintptr_t dst_texture,
    const TargetRectangle& src_rect, uintptr_t src_texture,
    const TargetSize& src_size, int src_layer, bool is_depth_texture = false,
    bool force_shader_copy = false) override;
  // Shadered shader stages
  const VkShaderModule& GetVertexShader(bool layered) const
  {
    return layered ? m_layered_vertex_shader : m_vertex_shader;
  }
  const VkShaderModule& GetGeometryShader() const
  {
    return m_layered_geometry_shader;
  }
  const VkSampler GetSamplerHandle(u32 idx) const
  {
    return m_samplers[idx];
  }
protected:
  bool CreateCommonShaders();

  std::unique_ptr<PostProcessingShader> CreateShader(PostProcessingShaderConfiguration* config) override;

  VkShaderModule m_vertex_shader;
  VkShaderModule m_layered_vertex_shader;
  VkShaderModule m_layered_geometry_shader;
  std::vector<VkSampler> m_samplers;
};

}  // namespace
