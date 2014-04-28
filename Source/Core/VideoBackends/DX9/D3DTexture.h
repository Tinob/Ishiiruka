// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "D3DBase.h"

namespace DX9
{

namespace D3D
{
	LPDIRECT3DTEXTURE9 CreateTexture2D(const u8* buffer, const s32 width, const s32 height, const s32 pitch, D3DFORMAT fmt = D3DFMT_A8R8G8B8, bool swap_r_b = false, s32 levels = 1);
	void ReplaceTexture2D(LPDIRECT3DTEXTURE9 pTexture, const u8* buffer, const s32 width, const s32 height, const s32 pitch, D3DFORMAT fmt, bool swap_r_b, s32 level = 0);
	LPDIRECT3DTEXTURE9 CreateRenderTarget(const s32 width, const s32 height);
	LPDIRECT3DSURFACE9 CreateDepthStencilSurface(const s32 width, const s32 height);
}

}  //  namespace DX9
