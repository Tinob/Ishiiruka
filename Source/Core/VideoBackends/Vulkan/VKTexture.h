// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <vulkan/vulkan.h>

#include "VideoCommon/HostTexture.h"

namespace Vulkan
{
class Texture2D;

class VKTexture final : public HostTexture
{
public:
  VKTexture() = delete;
  ~VKTexture();

  void Bind(u32 stage) override;
  bool Save(const std::string& filename, u32 level) override;

  void CopyRectangleFromTexture(const HostTexture* source,
    const MathUtil::Rectangle<int>& srcrect,
    const MathUtil::Rectangle<int>& dstrect) override;
  void CopyRectangleFromTexture(Texture2D* source, const MathUtil::Rectangle<int>& srcrect,
    const MathUtil::Rectangle<int>& dstrect);
  void Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level) override;

  Texture2D* GetRawTexIdentifier() const;
  VkFramebuffer GetFramebuffer() const;

  static std::unique_ptr<VKTexture> Create(const TextureConfig& tex_config);
  uintptr_t GetInternalObject() const override { return reinterpret_cast<uintptr_t>(m_texture.get()); };
private:
  VKTexture(const TextureConfig& tex_config, std::unique_ptr<Texture2D> texture);

  // Copies the contents of a texture using vkCmdCopyImage
  void CopyTextureRectangle(const MathUtil::Rectangle<int>& dst_rect, Texture2D* src_texture,
    const MathUtil::Rectangle<int>& src_rect);

  // Copies (and optionally scales) the contents of a texture using a framgent shader.
  void ScaleTextureRectangle(const MathUtil::Rectangle<int>& dst_rect, Texture2D* src_texture,
    const MathUtil::Rectangle<int>& src_rect);

  std::unique_ptr<Texture2D> m_texture;
  bool compressed = false;
};

}  // namespace Vulkan
