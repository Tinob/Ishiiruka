// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include "VideoCommon/TextureDecoder.h"
#include "VideoCommon/VideoCommon.h"

class HiresTexture
{
public:
	static void Init(const std::string& gameCode);
	
	static HiresTexture* Search(
		const u8* texture, size_t texture_size,
		const u8* tlut, size_t tlut_size,
		u32 width, u32 height,
		int format, 
		bool rgbaonly,
		std::function<u8*(size_t)> request_buffer_delegate
		);

	static std::string GenBaseName(
		const u8* texture, size_t texture_size,
		const u8* tlut, size_t tlut_size,
		u32 width, u32 height,
		int format
		);
	~HiresTexture(){};
	PC_TexFormat m_format;	
	u32 m_width, m_height, m_levels;
private:
	HiresTexture() : m_format(PC_TEX_FMT_NONE), m_height(0), m_levels(0){}
	typedef std::vector<std::pair<std::string, std::string>> HiresTextureCacheItem;
	typedef std::unordered_map<u64, HiresTextureCacheItem> HiresTextureCache;
	static HiresTextureCache textureMap;
};