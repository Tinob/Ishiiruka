// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once
#include "Common/Common.h"
#include "VideoCommon/TextureDecoder.h"

void TexDecoder_OpenCL_Initialize();
void TexDecoder_OpenCL_Shutdown();
PC_TexFormat TexDecoder_Decode_OpenCL(u8 *dst, const u8 *src, int width, int height, int texformat, int tlutaddr, TlutFormat tlutfmt, bool rgba);
