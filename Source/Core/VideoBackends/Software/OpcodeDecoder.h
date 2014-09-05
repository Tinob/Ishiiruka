// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#ifndef _OPCODEDECODER_H_
#define _OPCODEDECODER_H_

#include "Common/CommonTypes.h"
#include "Common/ChunkFile.h"
#include "VideoCommon/OpcodeDecoding.h"

namespace OpcodeDecoder
{
	void Init();

	void ResetDecoding();

	bool CommandRunnable(u32 iBufferSize);

	void Run(u32 iBufferSize);

	void DoState(PointerWrap &p);
}

#endif
