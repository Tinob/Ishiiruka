// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.
#pragma once 
#include "Common/CommonTypes.h"
#include "VideoCommon/DataReader.h"

void OpcodeDecoderSC_Init();
void OpcodeDecoderSC_Shutdown();
void OpcodeDecoderSC_Run(const u8* end);
void ExecuteDisplayListSC(u32 address, u32 size);

extern DataReader g_VideoDataSC;