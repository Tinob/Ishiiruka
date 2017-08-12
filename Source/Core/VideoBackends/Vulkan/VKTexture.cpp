// Copyright 2017 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <algorithm>
#include <cstddef>
#include <cstring>

#include "Common/Align.h"
#include "Common/Assert.h"
#include "Common/CommonTypes.h"
#include "Common/Logging/Log.h"
#include "Common/MsgHandler.h"

#include "VideoBackends/Vulkan/CommandBufferManager.h"
#include "VideoBackends/Vulkan/FramebufferManager.h"
#include "VideoBackends/Vulkan/StagingTexture2D.h"
#include "VideoBackends/Vulkan/StateTracker.h"
#include "VideoBackends/Vulkan/Texture2D.h"
#include "VideoBackends/Vulkan/Util.h"
#include "VideoBackends/Vulkan/VKTexture.h"
#include "VideoBackends/Vulkan/VulkanContext.h"

#include "VideoCommon/ImageWrite.h"
#include "VideoCommon/TextureConfig.h"

namespace Vulkan
{
VKTexture::VKTexture(const TextureConfig& tex_config, std::unique_ptr<Texture2D> texture)
  : HostTexture(tex_config), m_texture(std::move(texture))
{  
}

std::unique_ptr<VKTexture> VKTexture::Create(const TextureConfig& tex_config)
{
  static const VkFormat HostTextureFormat_To_VkFormat[]
  {
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_NONE
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_BGRA32
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_RGBA32
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_I4_AS_I8
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_IA4_AS_IA8
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_I8
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_IA8
    VK_FORMAT_R8G8B8A8_UNORM,//PC_TEX_FMT_RGB565
    VK_FORMAT_BC1_RGBA_UNORM_BLOCK,//PC_TEX_FMT_DXT1
    VK_FORMAT_BC2_UNORM_BLOCK,//PC_TEX_FMT_DXT3
    VK_FORMAT_BC3_UNORM_BLOCK,//PC_TEX_FMT_DXT5
    VK_FORMAT_BC7_UNORM_BLOCK,//PC_TEX_FMT_BPTC
    VK_FORMAT_R32_SFLOAT,//PC_TEX_FMT_DEPTH_FLOAT
    VK_FORMAT_R32_SFLOAT,//PC_TEX_FMT_R_FLOAT
    VK_FORMAT_R16G16B16A16_SFLOAT,//PC_TEX_FMT_RGBA16_FLOAT
    VK_FORMAT_R32G32B32A32_SFLOAT,//PC_TEX_FMT_RGBA_FLOAT
  };

  // Determine image usage, we need to flag as an attachment if it can be used as a rendertarget.
  VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
    VK_IMAGE_USAGE_SAMPLED_BIT;
  if (tex_config.rendertarget)
    usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  // Allocate texture object
  VkFormat vk_format = HostTextureFormat_To_VkFormat[tex_config.pcformat];
  auto texture = Texture2D::Create(tex_config.width, tex_config.height, tex_config.levels,
    tex_config.layers, vk_format, VK_SAMPLE_COUNT_1_BIT,
    VK_IMAGE_VIEW_TYPE_2D_ARRAY, VK_IMAGE_TILING_OPTIMAL, usage, TextureCache::GetInstance()->GetRenderPass());

  if (!texture)
  {
    return nullptr;
  }

  VKTexture* tex = new VKTexture(tex_config, std::move(texture));
  tex->compressed = TexDecoder::IsCompressed(tex_config.pcformat);
  return std::unique_ptr<VKTexture>(tex);
}

VKTexture::~VKTexture()
{
  // Texture is automatically cleaned up, however, we don't want to leave it bound.
  StateTracker::GetInstance()->UnbindTexture(m_texture->GetView());
}

Texture2D* VKTexture::GetRawTexIdentifier() const
{
  return m_texture.get();
}
VkFramebuffer VKTexture::GetFramebuffer() const
{
  return m_texture->GetFrameBuffer();
}

void VKTexture::Bind(u32 stage)
{
  // Texture should always be in SHADER_READ_ONLY layout prior to use.
  // This is so we don't need to transition during render passes.
  _assert_(m_texture->GetLayout() == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  StateTracker::GetInstance()->SetTexture(stage, m_texture->GetView());
}

bool VKTexture::Save(const std::string& filename, u32 level)
{
  _assert_(level < m_config.levels);

  // We can't dump compressed textures currently (it would mean drawing them to a RGBA8
  // framebuffer, and saving that). TextureCache does not call Save for custom textures
  // anyway, so this is fine for now.
  if (m_config.pcformat != HostTextureFormat::PC_TEX_FMT_RGBA32)
    return false;
  // Determine dimensions of image we want to save.
  u32 level_width = std::max(1u, m_config.width >> level);
  u32 level_height = std::max(1u, m_config.height >> level);

  // Use a temporary staging texture for the download. Certainly not optimal,
  // but since we have to idle the GPU anyway it doesn't really matter.
  std::unique_ptr<StagingTexture2D> staging_texture = StagingTexture2D::Create(
    STAGING_BUFFER_TYPE_READBACK, level_width, level_height, TEXTURECACHE_TEXTURE_FORMAT);

  // Transition image to transfer source, and invalidate the current state,
  // since we'll be executing the command buffer.
  m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  StateTracker::GetInstance()->EndRenderPass();

  // Copy to download buffer.
  staging_texture->CopyFromImage(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    m_texture->GetImage(), VK_IMAGE_ASPECT_COLOR_BIT, 0, 0,
    level_width, level_height, level, 0);

  // Restore original state of texture.
  m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  // Block until the GPU has finished copying to the staging texture.
  Util::ExecuteCurrentCommandsAndRestoreState(false, true);

  // Map the staging texture so we can copy the contents out.
  if (!staging_texture->Map())
  {
    PanicAlert("Failed to map staging texture");
    return false;
  }

  // Write texture out to file.
  // It's okay to throw this texture away immediately, since we're done with it, and
  // we blocked until the copy completed on the GPU anyway.
  bool result = TextureToPng(reinterpret_cast<u8*>(staging_texture->GetMapPointer()),
    static_cast<u32>(staging_texture->GetRowStride()), filename,
    level_width, level_height);

  staging_texture->Unmap();
  return result;
}

void VKTexture::CopyTextureRectangle(const MathUtil::Rectangle<int>& dst_rect,
  Texture2D* src_texture,
  const MathUtil::Rectangle<int>& src_rect)
{
  _assert_msg_(VIDEO, static_cast<u32>(src_rect.GetWidth()) <= src_texture->GetWidth() &&
    static_cast<u32>(src_rect.GetHeight()) <= src_texture->GetHeight(),
    "Source rect is too large for CopyRectangleFromTexture");

  _assert_msg_(VIDEO, static_cast<u32>(dst_rect.GetWidth()) <= m_config.width &&
    static_cast<u32>(dst_rect.GetHeight()) <= m_config.height,
    "Dest rect is too large for CopyRectangleFromTexture");

  VkImageCopy image_copy = {
    { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0,
    src_texture->GetLayers() },        // VkImageSubresourceLayers    srcSubresource
    { src_rect.left, src_rect.top, 0 },  // VkOffset3D                  srcOffset
    { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0,  // VkImageSubresourceLayers    dstSubresource
    m_config.layers },
    { dst_rect.left, dst_rect.top, 0 },  // VkOffset3D                  dstOffset
    { static_cast<uint32_t>(src_rect.GetWidth()), static_cast<uint32_t>(src_rect.GetHeight()),
    1 }  // VkExtent3D                  extent
  };

  // Must be called outside of a render pass.
  StateTracker::GetInstance()->EndRenderPass();

  src_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
  m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  vkCmdCopyImage(g_command_buffer_mgr->GetCurrentCommandBuffer(), src_texture->GetImage(),
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, m_texture->GetImage(),
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &image_copy);
}

void VKTexture::ScaleTextureRectangle(const MathUtil::Rectangle<int>& dst_rect,
  Texture2D* src_texture,
  const MathUtil::Rectangle<int>& src_rect)
{
  // Can't do this within a game render pass.
  StateTracker::GetInstance()->EndRenderPass();
  StateTracker::GetInstance()->SetPendingRebind();

  // Can't render to a non-rendertarget (no framebuffer).
  _assert_msg_(VIDEO, m_config.rendertarget,
    "Destination texture for partial copy is not a rendertarget");

  // Render pass expects dst_texture to be in COLOR_ATTACHMENT_OPTIMAL state.
  // src_texture should already be in SHADER_READ_ONLY state, but transition in case (XFB).
  src_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

  UtilityShaderDraw draw(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    g_object_cache->GetPipelineLayout(PIPELINE_LAYOUT_STANDARD),
    TextureCache::GetInstance()->GetRenderPass(),
    g_object_cache->GetPassthroughVertexShader(),
    g_object_cache->GetPassthroughGeometryShader(),
    TextureCache::GetInstance()->GetCopyShader());

  VkRect2D region = {
    { dst_rect.left, dst_rect.top },
    { static_cast<u32>(dst_rect.GetWidth()), static_cast<u32>(dst_rect.GetHeight()) } };
  draw.BeginRenderPass(m_texture->GetFrameBuffer(), region);
  draw.SetPSSampler(0, src_texture->GetView(), g_object_cache->GetLinearSampler());
  draw.DrawQuad(dst_rect.left, dst_rect.top, dst_rect.GetWidth(), dst_rect.GetHeight(),
    src_rect.left, src_rect.top, 0, src_rect.GetWidth(), src_rect.GetHeight(),
    static_cast<int>(src_texture->GetWidth()),
    static_cast<int>(src_texture->GetHeight()));
  draw.EndRenderPass();
}

void VKTexture::CopyRectangleFromTexture(const HostTexture* source,
  const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
  auto* raw_source_texture = static_cast<const VKTexture*>(source)->GetRawTexIdentifier();
  CopyRectangleFromTexture(raw_source_texture, srcrect, dstrect);
}

void VKTexture::CopyRectangleFromTexture(Texture2D* source, const MathUtil::Rectangle<int>& srcrect,
  const MathUtil::Rectangle<int>& dstrect)
{
  if (srcrect.GetWidth() == dstrect.GetWidth() && srcrect.GetHeight() == dstrect.GetHeight())
    CopyTextureRectangle(dstrect, source, srcrect);
  else
    ScaleTextureRectangle(dstrect, source, srcrect);

  // Ensure both textures remain in the SHADER_READ_ONLY layout so they can be bound.
  source->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentCommandBuffer(),
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void VKTexture::Load(const u8* src, u32 width, u32 height, u32 expanded_width, u32 level)
{
  // Can't copy data larger than the texture extents.
  width = std::max(1u, std::min(width, m_config.width >> level));
  height = std::max(1u, std::min(height, m_config.height >> level));
  u32 block_size = sizeof(u32);
  u32 block_W = width;
  u32 block_stride = expanded_width;
  u32 block_H = height;
  auto format = m_texture->GetFormat();
  if (format != VK_FORMAT_R8G8B8A8_UNORM
    && format != VK_FORMAT_R32_SFLOAT)
  {
    if (format == VK_FORMAT_BC1_RGBA_UNORM_BLOCK)
    {
      block_size = 8;
    }
    else
    {
      block_size = 16;
    }
    block_W = std::max(1u, (block_W + 3) >> 2);
    block_H = std::max(1u, (block_H + 3) >> 2);
    block_stride = std::max(1u, (block_stride + 3) >> 2);
  }
  // We don't care about the existing contents of the texture, so we could the image layout to
  // VK_IMAGE_LAYOUT_UNDEFINED here. However, under section 2.2.1, Queue Operation of the Vulkan
  // specification, it states:
  //
  //   Command buffer submissions to a single queue must always adhere to command order and
  //   API order, but otherwise may overlap or execute out of order.
  //
  // Therefore, if a previous frame's command buffer is still sampling from this texture, and we
  // overwrite it without a pipeline barrier, a texture sample could occur in parallel with the
  // texture upload/copy. I'm not sure if any drivers currently take advantage of this, but we
  // should insert an explicit pipeline barrier just in case (done by TransitionToLayout).
  //
  // We transition to TRANSFER_DST, ready for the image copy, and leave the texture in this state.
  // This is so that the remaining mip levels can be uploaded without barriers, and then when the
  // texture is used, it can be transitioned to SHADER_READ_ONLY (see TCacheEntry::Bind).
  m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentInitCommandBuffer(),
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

  // Does this texture data fit within the streaming buffer?
  u32 upload_width = block_W;
  u32 upload_pitch = upload_width * block_size;
  u32 upload_size = upload_pitch * block_H;
  u32 upload_alignment = static_cast<u32>(g_vulkan_context->GetBufferImageGranularity());
  u32 source_pitch = block_stride * block_size;
  upload_alignment = block_size > upload_alignment ? block_size : upload_alignment;
  if ((upload_size + upload_alignment) <= STAGING_TEXTURE_UPLOAD_THRESHOLD &&
    (upload_size + upload_alignment) <= MAXIMUM_TEXTURE_UPLOAD_BUFFER_SIZE)
  {
    // Assume tightly packed rows, with no padding as the buffer source.
    StreamBuffer* upload_buffer = TextureCache::GetInstance()->GetTextureUploadBuffer();

    // Allocate memory from the streaming buffer for the texture data.
    if (!upload_buffer->ReserveMemory(upload_size, upload_alignment))
    {
      // Execute the command buffer first.
      WARN_LOG(VIDEO, "Executing command list while waiting for space in texture upload buffer");
      Util::ExecuteCurrentCommandsAndRestoreState(false);

      // Try allocating again. This may cause a fence wait.
      if (!upload_buffer->ReserveMemory(upload_size, upload_alignment))
        PanicAlert("Failed to allocate space in texture upload buffer");
    }

    // Grab buffer pointers
    VkBuffer image_upload_buffer = upload_buffer->GetBuffer();
    VkDeviceSize image_upload_buffer_offset = upload_buffer->GetCurrentOffset();
    u8* image_upload_buffer_pointer = upload_buffer->GetCurrentHostPointer();

    // Copy to the buffer using the stride from the subresource layout
    const u8* source_ptr = src;
    if (upload_pitch != source_pitch)
    {
      VkDeviceSize copy_pitch = std::min(source_pitch, upload_pitch);
      for (u32 row = 0; row < block_H; row++)
      {
        memcpy(image_upload_buffer_pointer + row * upload_pitch, source_ptr + row * source_pitch,
          copy_pitch);
      }
    }
    else
    {
      // Can copy the whole thing in one block, the pitch matches
      memcpy(image_upload_buffer_pointer, source_ptr, upload_size);
    }

    // Flush buffer memory if necessary
    upload_buffer->CommitMemory(upload_size);

    // Copy from the streaming buffer to the actual image.
    VkBufferImageCopy image_copy = {
      image_upload_buffer_offset,                // VkDeviceSize                bufferOffset
      0,                                         // uint32_t                    bufferRowLength
      0,                                         // uint32_t                    bufferImageHeight
      { VK_IMAGE_ASPECT_COLOR_BIT, level, 0, 1 },  // VkImageSubresourceLayers    imageSubresource
      { 0, 0, 0 },                                 // VkOffset3D                  imageOffset
      { width, height, 1 }                         // VkExtent3D                  imageExtent
    };
    vkCmdCopyBufferToImage(g_command_buffer_mgr->GetCurrentInitCommandBuffer(), image_upload_buffer,
      m_texture->GetImage(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
      &image_copy);
  }
  else
  {
    // Slow path. The data for the image is too large to fit in the streaming buffer, so we need
    // to allocate a temporary texture to store the data in, then copy to the real texture.
    std::unique_ptr<StagingTexture2D> staging_texture = StagingTexture2D::Create(
      STAGING_BUFFER_TYPE_UPLOAD, width, height, m_texture->GetFormat());

    if (!staging_texture || !staging_texture->Map())
    {
      PanicAlert("Failed to allocate staging texture for large texture upload.");
      return;
    }

    // Copy data to staging texture first, then to the "real" texture.
    staging_texture->WriteTexels(0, 0, width, height, src, source_pitch);
    staging_texture->CopyToImage(g_command_buffer_mgr->GetCurrentInitCommandBuffer(),
      m_texture->GetImage(), VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, width,
      height, level, 0);
  }
  // Last mip level? We shouldn't be doing any further uploads now, so transition for rendering.
  if (level == (m_config.levels - 1))
  {
    m_texture->TransitionToLayout(g_command_buffer_mgr->GetCurrentInitCommandBuffer(),
      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
  }
}

}  // namespace Vulkan
