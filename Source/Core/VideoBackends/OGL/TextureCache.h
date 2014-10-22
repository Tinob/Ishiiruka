// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#ifndef _TEXTUREMNGR_H_
#define _TEXTUREMNGR_H_

#include <map>

#include "VideoCommon/VideoCommon.h"
#include "GLUtil.h"
#include "VideoCommon/BPStructs.h"

#include "VideoCommon/TextureCacheBase.h"

namespace OGL
{

class TextureCache : public ::TextureCache
{
public:
	TextureCache();
	static void DisableStage(unsigned int stage);
	static void SetStage();
	static void SetNextStage(unsigned int stage);

private:
	struct TCacheEntry : TCacheEntryBase
	{
		GLuint texture;
		GLuint framebuffer;
		bool compressed;

		int gl_format;
		int gl_iformat;
		int gl_type;

		int m_tex_levels;

		//TexMode0 mode; // current filter and clamp modes that texture is set to
		//TexMode1 mode1; // current filter and clamp modes that texture is set to

		TCacheEntry();
		~TCacheEntry();

		void Load(const u8* src, u32 width, u32 height,
			u32 expanded_width, u32 level);
		void Load(const u8* src, u32 width, u32 height, u32 expandedWidth,
			u32 expandedHeight, const s32 texformat, const u32 tlutaddr, const s32 tlutfmt, u32 level);
		void LoadFromTmem(const u8* ar_src, const u8* gb_src, u32 width, u32 height,
			u32 expanded_width, u32 expanded_Height, u32 level);

		void FromRenderTarget(u32 dstAddr, unsigned int dstFormat,
			unsigned int srcFormat, const EFBRectangle& srcRect,
			bool isIntensity, bool scaleByHalf, unsigned int cbufid,
			const float *colmat);

		void Bind(unsigned int stage);
		bool Save(const char filename[], unsigned int level);
	};

	~TextureCache();

	PC_TexFormat GetNativeTextureFormat(const s32 texformat, const s32 tlutfmt, u32 width, u32 height);

	TCacheEntryBase* CreateTexture(u32 width, u32 height,
		u32 expanded_width, u32 tex_levels, PC_TexFormat pcfmt);

	TCacheEntryBase* CreateRenderTargetTexture(unsigned int scaled_tex_w, unsigned int scaled_tex_h);
};

bool SaveTexture(const char* filename, u32 textarget, u32 tex, int virtual_width, int virtual_height, unsigned int level);

}

#endif // _TEXTUREMNGR_H_
