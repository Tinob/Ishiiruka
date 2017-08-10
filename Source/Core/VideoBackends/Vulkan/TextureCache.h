// Copyright 2016 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <memory>

#include "Common/CommonTypes.h"
#include "VideoBackends/Vulkan/StreamBuffer.h"
#include "VideoCommon/TextureCacheBase.h"
#include "VideoCommon/TextureScalerCommon.h"

namespace Vulkan
{
class TextureConverter;
class StateTracker;
class Texture2D;

class TextureCache : public TextureCacheBase
{
public:
	TextureCache();
	~TextureCache();

	void LoadData(Texture2D* dst, const u8* src, u32 width, u32 height,
		u32 expanded_width, u32 level);

	static TextureCache* GetInstance();
	TextureConverter* GetTextureConverter() const { return m_texture_converter.get(); }
	bool Initialize();

	bool CompileShaders() override;
	void DeleteShaders() override;

    std::unique_ptr<HostTexture> CreateTexture(const TextureConfig& config) override;

    void CopyEFBToCacheEntry(TextureCacheBase::TCacheEntry* entry, bool is_depth_copy, const EFBRectangle& src_rect,
      bool scale_by_half, u32 cbuf_id, const float* colmat, u32 width, u32 height) override;

    bool DecodeTextureOnGPU(HostTexture* dst, u32 dst_level, const u8* data,
      u32 data_size, TextureFormat format, u32 width, u32 height,
      u32 aligned_width, u32 aligned_height, u32 row_stride,
      const u8* palette, TlutFormat palette_format) override;

	void CopyEFB(u8* dst, const EFBCopyFormat& format, u32 native_width, u32 bytes_per_row,
		u32 num_blocks_y, u32 memory_stride,
		bool is_depth_copy, const EFBRectangle& src_rect, bool scale_by_half) override;

	bool SupportsGPUTextureDecode(TextureFormat format, TlutFormat palette_format) override;
	TextureConverter* GetTextureConverter()
	{
		return m_texture_converter.get();
	}
	VkRenderPass GetRenderPass() const
	{
		return m_render_pass;
	}
	VkShaderModule GetCopyShader() const
	{
		return m_copy_shader;
	}
    StreamBuffer* GetTextureUploadBuffer() const
    {
      return m_texture_upload_buffer.get();
    }
private:
	bool CreateRenderPasses();

	VkRenderPass m_render_pass = VK_NULL_HANDLE;

	std::unique_ptr<StreamBuffer> m_texture_upload_buffer;

	std::unique_ptr<TextureConverter> m_texture_converter;

	std::unique_ptr<TextureScaler> m_scaler;

	VkShaderModule m_copy_shader = VK_NULL_HANDLE;
	VkShaderModule m_efb_color_to_tex_shader = VK_NULL_HANDLE;
	VkShaderModule m_efb_depth_to_tex_shader = VK_NULL_HANDLE;

	void* m_pallette;
	TlutFormat m_pallette_format;
	u32 m_pallette_size;
	bool Palettize(TCacheEntry* entry, const TCacheEntry* base_entry) override;
	void LoadLut(u32 lutFmt, void* addr, u32 size) override;
	HostTextureFormat GetHostTextureFormat(const s32 texformat, const TlutFormat tlutfmt, u32 width, u32 height) override;
};

}  // namespace Vulkan
