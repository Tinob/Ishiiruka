// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#ifndef _TEXTURECACHE_H
#define _TEXTURECACHE_H


#include <map>

#include "D3DBase.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/BPMemory.h"

#include "VideoCommon/TextureCacheBase.h"

namespace DX9
{

class TextureCache : public ::TextureCache
{
private:
	struct TCacheEntry : TCacheEntryBase
	{
		const LPDIRECT3DTEXTURE9 texture;

		D3DFORMAT d3d_fmt;
		bool swap_r_b;
		bool compressed;
		TCacheEntry(LPDIRECT3DTEXTURE9 _tex) : texture(_tex) {}
		~TCacheEntry();

		void Load(const u8* src, u32 width, u32 height,
			u32 expanded_width, u32 level);
		void Load(const u8* src, u32 width, u32 height, u32 expandedWidth,
			u32 expandedHeight, const s32 texformat, const u32 tlutaddr, const s32 tlutfmt, u32 level);
		void LoadFromTmem(const u8* ar_src, const u8* gb_src, u32 width, u32 height,
			u32 expanded_width, u32 expanded_Height, u32 level);

		void FromRenderTarget(u32 dstAddr, u32 dstFormat,
			u32 srcFormat, const EFBRectangle& srcRect,
			bool isIntensity, bool scaleByHalf, u32 cbufid,
			const float *colmat);

		void Bind(u32 stage);
		bool Save(const char filename[], u32 level);
	};

	PC_TexFormat GetNativeTextureFormat(const s32 texformat, const s32 tlutfmt, u32 width, u32 height);

	TCacheEntryBase* CreateTexture(u32 width, u32 height,
		u32 expanded_width, u32 tex_levels, PC_TexFormat pcfmt);

	TCacheEntryBase* CreateRenderTargetTexture(u32 scaled_tex_w, u32 scaled_tex_h);
};

}

#endif
