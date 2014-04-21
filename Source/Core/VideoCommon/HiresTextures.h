// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#ifndef _HIRESTEXTURES_H
#define _HIRESTEXTURES_H

#include <map>
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/TextureDecoder.h"

namespace HiresTextures
{
void Init(const char *gameCode);
bool HiresTexExists(const char *filename);
PC_TexFormat GetHiresTex(const char *fileName, u32 *pWidth, u32 *pHeight, u32 *required_size, u32 *numMips, s32 texformat, u32 data_size, u8 *dst);

};

#endif // _HIRESTEXTURES_H
