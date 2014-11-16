#pragma once

#include "Common/CommonTypes.h"
#include "Common/Flag.h"

extern bool s_BackendInitialized;
extern Common::Flag s_swapRequested;
extern volatile u32 s_EFB_PCache_Frame;

void VideoFifo_CheckEFBAccess();
void VideoFifo_CheckSwapRequestAt(u32 xfbAddr, u32 fbWidth, u32 fbHeight);