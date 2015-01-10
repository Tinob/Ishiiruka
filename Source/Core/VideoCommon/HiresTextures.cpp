// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>
#include <SOIL/SOIL.h>

#include "Common/CommonPaths.h"
#include "Common/FileSearch.h"
#include "Common/FileUtil.h"
#include "Common/StringUtil.h"

#include "Core/ConfigManager.h"

#include "VideoCommon/DDSLoader.h"
#include "VideoCommon/HiresTextures.h"
#include "VideoCommon/TextureUtil.h"
#include "VideoCommon/VideoConfig.h"

HiresTexture::HiresTextureCache HiresTexture::textureMap;

__forceinline u64 GetTextureKey(u64 Hash, u64 format)
{
	return  (Hash << 32) | format;
}

__forceinline u64 GetTextureHash(const u8* texture, size_t texture_size, const u8* tlut, size_t tlut_size)
{
	u64 tex_hash = GetHashHiresTexture(texture, (int)texture_size, g_ActiveConfig.iSafeTextureCache_ColorSamples);
	if (tlut_size)
	{
		u64 tlut_hash = GetHashHiresTexture(tlut, (int)tlut_size, g_ActiveConfig.iSafeTextureCache_ColorSamples);
		tex_hash ^= tlut_hash;
	}
	return tex_hash;
}

void HiresTexture::Init(const std::string& gameCode)
{
	textureMap.clear();
	CFileSearch::XStringVector Directories;
	std::string szDir = StringFromFormat("%s%s", File::GetUserPath(D_HIRESTEXTURES_IDX).c_str(), gameCode.c_str());
	Directories.push_back(szDir);

	for (u32 i = 0; i < Directories.size(); i++)
	{
		File::FSTEntry FST_Temp;
		File::ScanDirectoryTree(Directories[i], FST_Temp);
		for (u32 j = 0; j < FST_Temp.children.size(); j++)
		{
			if (FST_Temp.children.at(j).isDirectory)
			{
				bool duplicate = false;
				for (u32 k = 0; k < Directories.size(); k++)
				{
					if (strcmp(Directories[k].c_str(), FST_Temp.children.at(j).physicalName.c_str()) == 0)
					{
						duplicate = true;
						break;
					}
				}
				if (!duplicate)
					Directories.push_back(FST_Temp.children.at(j).physicalName.c_str());
			}
		}
	}

	CFileSearch::XStringVector Extensions;
	Extensions.push_back("*.png");
	Extensions.push_back("*.bmp");
	Extensions.push_back("*.tga");
	Extensions.push_back("*.dds");
	Extensions.push_back("*.jpg"); // Why not? Could be useful for large photo-like textures
	CFileSearch FileSearch(Extensions, Directories);
	const CFileSearch::XStringVector& rFilenames = FileSearch.GetFileNames();
	std::string code(gameCode);

	if (rFilenames.size() > 0)
	{
		for (u32 i = 0; i < rFilenames.size(); i++)
		{
			std::string FileName;
			std::string Extension;			
			SplitPath(rFilenames[i], NULL, &FileName, &Extension);
			std::pair<std::string, std::string> Pair(rFilenames[i], Extension);
			std::vector<std::string> nameparts;
			std::istringstream issfilename(FileName);
			std::string nameitem;
			while (std::getline(issfilename, nameitem, '_')) {
				nameparts.push_back(nameitem);
			}
			if (nameparts.size() >= 3)
			{
				if (nameparts[0].compare(code) != 0)
				{
					continue;
				}
				u32 hash = 0;
				u32 format = 0;
				u32 level = 0;
				sscanf(nameparts[1].c_str(), "%x", &hash);
				sscanf(nameparts[2].c_str(), "%i", &format);
				if (nameparts.size() > 3 && nameparts[3].size() > 3)
				{
					sscanf(nameparts[3].substr(3, std::string::npos).c_str(), "%i", &level);
				}
				u64 key = GetTextureKey(hash, format);
				HiresTextureCache::iterator iter = textureMap.find(key);
				u32 min_item_size = level + 1;
				if (iter == textureMap.end())
				{
					HiresTextureCacheItem item(min_item_size);
					item[level] = Pair;
					textureMap.insert(HiresTextureCache::value_type(key, item));
				}
				else
				{
					if (iter->second.size() < min_item_size)
					{
						iter->second.resize(min_item_size);
					}
					iter->second[level] = Pair;
				}
			}
		}
	}
}

std::string HiresTexture::GenBaseName(const u8* texture, size_t texture_size, const u8* tlut, size_t tlut_size, u32 width, u32 height, int format)
{
	u64 hash = GetTextureHash(texture, texture_size, tlut, tlut_size);
	return StringFromFormat("%s_%08x_%i", SConfig::GetInstance().m_LocalCoreStartupParameter.m_strUniqueID.c_str(), (u32)hash, (u16)format);
}



inline void LoadImageFromFile_Soil(ImageLoaderParams &ImgInfo)
{
	int width;
	int height;
	int channels;
	ImgInfo.resultTex = PC_TEX_FMT_NONE;
	u8* temp = SOIL_load_image(ImgInfo.Path, &width, &height, &channels, ImgInfo.forcedchannels);
	if (temp == nullptr)
	{
		return;
	}
	ImgInfo.Width = width;
	ImgInfo.Height = height;
	ImgInfo.data_size = width * height * ImgInfo.formatBPP;
	ImgInfo.dst = ImgInfo.request_buffer_delegate(ImgInfo.data_size);
	if (ImgInfo.dst != nullptr)
	{
		ImgInfo.resultTex = ImgInfo.desiredTex;
		memcpy(ImgInfo.dst, temp, ImgInfo.data_size);
	}
	SOIL_free_image_data(temp);
	return;
}


inline void LoadImageFromFile_DDS(ImageLoaderParams &ImgInfo)
{
	DDSCompression ddsc = DDSLoader::Load_Image(ImgInfo);
	if (ddsc != DDSCompression::DDSC_NONE)
	{
		switch (ddsc)
		{
		case DDSC_DXT1:
			ImgInfo.resultTex = PC_TEX_FMT_DXT1;
			break;
		case DDSC_DXT3:
			ImgInfo.resultTex = PC_TEX_FMT_DXT3;
			break;
		case DDSC_DXT5:
			ImgInfo.resultTex = PC_TEX_FMT_DXT5;
			break;
		default:
			break;
		}
	}
}

HiresTexture* HiresTexture::Search(
	const u8* texture, 
	size_t texture_size, 
	const u8* tlut, 
	size_t tlut_size, 
	u32 width, 
	u32 height, 
	s32 format,
	bool rgbaonly,
	std::function<u8*(size_t)> request_buffer_delegate)
{
	if (textureMap.size() == 0)
	{
		return nullptr;
	}
	u64 hash = GetTextureHash(texture, texture_size, tlut, tlut_size);
	u64 key = GetTextureKey(hash, format);
	HiresTextureCache::iterator iter = textureMap.find(key);
	if (iter == textureMap.end())
	{
		return nullptr;
	}
	HiresTextureCacheItem& current = iter->second;
	if (current.size() == 0)
	{
		return nullptr;
	}
	// First level is mandatory
	if (current[0].first.size() == 0)
	{
		return nullptr;
	}
	HiresTexture* ret = nullptr;	
	std::string ddscode(".dds");
	std::string cddscode(".DDS");
	u8* buffer_pointer;
	u32 maxwidth;
	u32 maxheight;
	for (size_t level = 0; level < current.size(); level++)
	{
		ImageLoaderParams imgInfo;
		imgInfo.dst = nullptr;
		imgInfo.Path = current[0].first.c_str();
		if (level == 0)
		{
			imgInfo.request_buffer_delegate = [&](size_t requiredsize)
			{
				// Pre allocate space for the textures and potetially all the posible mip levels 
				return request_buffer_delegate(requiredsize * 2);
			};
		}
		else
		{
			imgInfo.request_buffer_delegate = [&](size_t requiredsize)
			{
				// just return the pointer to pack the textures in a single buffer.
				return buffer_pointer;
			};
		}
		if (current[0].second.compare(ddscode) == 0 || current[0].second.compare(cddscode) == 0)
		{
			LoadImageFromFile_DDS(imgInfo);
		}
		else
		{
			format = rgbaonly ? GX_TF_RGBA8 : format;
			switch (format)
			{
			case GX_TF_IA4:
			case GX_TF_IA8:
				imgInfo.desiredTex = PC_TEX_FMT_IA8;
				imgInfo.forcedchannels = SOIL_LOAD_LA;
				imgInfo.formatBPP = 2;
				break;
			default:
				imgInfo.desiredTex = PC_TEX_FMT_RGBA32;
				imgInfo.forcedchannels = SOIL_LOAD_RGBA;
				imgInfo.formatBPP = 4;
				break;
			}
			LoadImageFromFile_Soil(imgInfo);
		}
		if (imgInfo.dst == nullptr || imgInfo.resultTex == PC_TEX_FMT_NONE)
		{
			ERROR_LOG(VIDEO, "Custom texture %s failed to load level %i", imgInfo.Path, level);
			break;
		}
		if (level == 0)
		{
			ret = new HiresTexture();
			ret->m_format = imgInfo.resultTex;			
			ret->m_width = maxwidth = imgInfo.Width;
			ret->m_height = maxheight = imgInfo.Height;
		}
		else
		{
			if (maxwidth != imgInfo.Width
				|| maxheight != imgInfo.Height
				|| ret->m_format != imgInfo.resultTex)
			{
				ERROR_LOG(VIDEO, 
					"Custom texture %s invalid level %i size: %i %i required: %i %i format: %i", 
					imgInfo.Path, 
					level, 
					imgInfo.Width, 
					imgInfo.Height, 
					maxwidth, 
					maxheight,
					ret->m_format);
				break;
			}
		}
		ret->m_levels++;
		buffer_pointer = imgInfo.dst + imgInfo.data_size;
		maxwidth = maxwidth >> 1;
		maxheight = maxheight >> 1;
		if (imgInfo.nummipmaps > 0)
		{
			// Using a dds with packed levels
			// Generate Levels
			ret->m_levels = imgInfo.nummipmaps;
			break;
		}
	}
	return ret;
}