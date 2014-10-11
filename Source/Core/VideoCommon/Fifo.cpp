// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/Atomic.h"
#include "Common/ChunkFile.h"
#include "Common/FPURoundMode.h"
#include "Common/MemoryUtil.h"
#include "Common/Thread.h"

#include "Core/Core.h"
#include "Core/ConfigManager.h"
#include "Core/CoreTiming.h"
#include "Core/HW/Memmap.h"

#include "VideoCommon/CommandProcessor.h"
#include "VideoCommon/DataReader.h"
#include "VideoCommon/Fifo.h"
#include "VideoCommon/HLSLCompiler.h"
#include "VideoCommon/OpcodeDecoding.h"
#include "VideoCommon/OpcodeDecodingSC.h"
#include "VideoCommon/PixelEngine.h"
#include "VideoCommon/VideoConfig.h"

bool g_bSkipCurrentFrame = false;

namespace
{
static volatile bool GpuRunningState = false;
static volatile bool EmuRunningState = false;
static std::mutex m_csHWVidOccupied;
// STATE_TO_SAVE
static u8 *videoBuffer;
static int size = 0;
static u8 *videoBufferSC;
static int sizeSC = 0;

volatile u32 CPReadFifoSC;
volatile u32 CPBaseFifoSC;
volatile u32 CPEndFifoSC;
}  // namespace

void Fifo_DoState(PointerWrap &p) 
{
	p.DoArray(videoBuffer, FIFO_SIZE);
	p.Do(size);
	u8* videodata = const_cast<u8*>(g_VideoData.GetReadPosition());
	p.DoPointer(videodata, videoBuffer);
	g_VideoData.SetReadPosition(videodata);
	p.Do(g_bSkipCurrentFrame);
	p.DoArray(videoBufferSC, FIFO_SIZE);
	p.Do(sizeSC);
	videodata = const_cast<u8*>(g_VideoDataSC.GetReadPosition());
	p.DoPointer(videodata, videoBufferSC);
	g_VideoDataSC.SetReadPosition(videodata);
	p.Do(CPReadFifoSC);
	p.Do(CPBaseFifoSC);
	p.Do(CPEndFifoSC);
}

void Fifo_PauseAndLock(bool doLock, bool unpauseOnUnlock)
{
	if (doLock)
	{
		EmulatorState(false);
		if (!Core::IsGPUThread())
			m_csHWVidOccupied.lock();
		_dbg_assert_(COMMON, !CommandProcessor::fifo.isGpuReadingData);
	}
	else
	{
		if (unpauseOnUnlock)
			EmulatorState(true);
		if (!Core::IsGPUThread())
			m_csHWVidOccupied.unlock();
	}
}


void Fifo_Init()
{
	videoBuffer = (u8*)AllocateMemoryPages(FIFO_SIZE);
	size = 0;
	GpuRunningState = false;
	Common::AtomicStore(CommandProcessor::VITicks, CommandProcessor::m_cpClockOrigin);
	videoBufferSC = (u8*)AllocateMemoryPages(FIFO_SIZE);
	sizeSC = 0;
	CPReadFifoSC = -1;
	CPBaseFifoSC = -1;
	CPEndFifoSC = -1;
}

void Fifo_Shutdown()
{
	if (GpuRunningState) PanicAlert("Fifo shutting down while active");
	FreeMemoryPages(videoBuffer, FIFO_SIZE);
	FreeMemoryPages(videoBufferSC, FIFO_SIZE);
	videoBuffer = nullptr;
	videoBufferSC = nullptr;
}

u8* GetVideoBufferStartPtr()
{
	return videoBuffer;
}

u8* GetVideoBufferEndPtr()
{
	return &videoBuffer[size];
}

u8* GetVideoBufferStartPtrSC()
{
	return videoBufferSC;
}

u8* GetVideoBufferEndPtrSC()
{
	return &videoBufferSC[sizeSC];
}


void Fifo_SetRendering(bool enabled)
{
	g_bSkipCurrentFrame = !enabled;
}

// May be executed from any thread, even the graphics thread.
// Created to allow for self shutdown.
void ExitGpuLoop()
{
	// This should break the wait loop in CPU thread
	CommandProcessor::fifo.bFF_GPReadEnable = false;
	SCPFifoStruct &fifo = CommandProcessor::fifo;
	while(fifo.isGpuReadingData) Common::YieldCPU();
	// Terminate GPU thread loop
	GpuRunningState = false;
	EmuRunningState = true;
}

void EmulatorState(bool running)
{
	EmuRunningState = running;
}


// Description: RunGpuLoop() sends data through this function.
void ReadDataFromFifo(u8* _uData, u32 len)
{
	if (size + len >= FIFO_SIZE)
	{
		int pos = (int)(g_VideoData.GetReadPosition() - videoBuffer);
		size -= pos;
		if (size + len > FIFO_SIZE)
		{
			PanicAlert("FIFO out of bounds (size = %i, len = %i at %08x)", size, len, pos);
		}
		memmove(&videoBuffer[0], &videoBuffer[pos], size);
		g_VideoData.SetReadPosition(videoBuffer);
	}
	// Copy new video instructions to videoBuffer for future use in rendering the new picture
	memcpy(videoBuffer + size, _uData, len);
	size += len;
}

void ResetVideoBuffer()
{
	g_VideoData.SetReadPosition(videoBuffer);
	size = 0;
	g_VideoDataSC.SetReadPosition(videoBufferSC);
	sizeSC = 0;

	CPReadFifoSC = -1;
	CPBaseFifoSC = -1;
	CPEndFifoSC = -1;
}


// Description: Main FIFO update loop
// Purpose: Keep the Core HW updated about the CPU-GPU distance
void RunGpuLoop()
{
	std::lock_guard<std::mutex> lk(m_csHWVidOccupied);
	GpuRunningState = true;
	SCPFifoStruct &fifo = CommandProcessor::fifo;
	u32 cyclesExecuted = 0;

	while (GpuRunningState)
	{
		g_video_backend->PeekMessages();

		VideoFifo_CheckAsyncRequest();

		CommandProcessor::SetCpStatus();

		Common::AtomicStore(CommandProcessor::VITicks, CommandProcessor::m_cpClockOrigin);

		// check if we are able to run this buffer	
		while (GpuRunningState && EmuRunningState && !CommandProcessor::interruptWaiting && fifo.bFF_GPReadEnable && fifo.CPReadWriteDistance && !AtBreakpoint())
		{
			fifo.isGpuReadingData = true;
			CommandProcessor::isPossibleWaitingSetDrawDone = fifo.bFF_GPLinkEnable ? true : false;

			if (!SConfig::GetInstance().m_LocalCoreStartupParameter.bSyncGPU || Common::AtomicLoad(CommandProcessor::VITicks) > CommandProcessor::m_cpClockOrigin)
			{
				u32 readPtr = fifo.CPReadPointer;
				u8 *uData = Memory::GetPointer(readPtr);

				if (readPtr == fifo.CPEnd)
					readPtr = fifo.CPBase;
				else
					readPtr += 32;

				_assert_msg_(COMMANDPROCESSOR, (s32)fifo.CPReadWriteDistance - 32 >= 0 ,
					"Negative fifo.CPReadWriteDistance = %i in FIFO Loop !\nThat can produce instability in the game. Please report it.", fifo.CPReadWriteDistance - 32);

				ReadDataFromFifo(uData, 32);

				cyclesExecuted = OpcodeDecoder_Run(GetVideoBufferEndPtr());

				if (SConfig::GetInstance().m_LocalCoreStartupParameter.bSyncGPU && Common::AtomicLoad(CommandProcessor::VITicks) > cyclesExecuted)
						Common::AtomicAdd(CommandProcessor::VITicks, -(s32)cyclesExecuted);

				Common::AtomicStore(fifo.CPReadPointer, readPtr);
				Common::AtomicAdd(fifo.CPReadWriteDistance, -32);
				if ((GetVideoBufferEndPtr() - g_VideoData.GetReadPosition()) == 0)
					Common::AtomicStore(fifo.SafeCPReadPointer, fifo.CPReadPointer);
			}

			CommandProcessor::SetCpStatus();
		
			// This call is pretty important in DualCore mode and must be called in the FIFO Loop.
			// If we don't, s_swapRequested or s_efbAccessRequested won't be set to false
			// leading the CPU thread to wait in Video_BeginField or Video_AccessEFB thus slowing things down.
			VideoFifo_CheckAsyncRequest();		
			CommandProcessor::isPossibleWaitingSetDrawDone = false;
		}

		fifo.isGpuReadingData = false;

		if (EmuRunningState)
		{
			// NOTE(jsd): Calling SwitchToThread() on Windows 7 x64 is a hot spot, according to profiler.
			// See https://docs.google.com/spreadsheet/ccc?key=0Ah4nh0yGtjrgdFpDeF9pS3V6RUotRVE3S3J4TGM1NlE#gid=0
			// for benchmark details.
#if 0
			Common::YieldCPU();
#endif
		}
		else
		{
			// While the emu is paused, we still handle async requests then sleep.
			while (!EmuRunningState)
			{
				g_video_backend->PeekMessages();
				m_csHWVidOccupied.unlock();
				Common::SleepCurrentThread(1);
				m_csHWVidOccupied.lock();
			}
		}
	}
}


bool AtBreakpoint()
{
	SCPFifoStruct &fifo = CommandProcessor::fifo;
	return fifo.bFF_BPEnable && (fifo.CPReadPointer == fifo.CPBreakpoint);
}

void RunGpu()
{
	SCPFifoStruct &fifo = CommandProcessor::fifo;
	while (fifo.bFF_GPReadEnable && fifo.CPReadWriteDistance && !AtBreakpoint() )
	{
		u8 *uData = Memory::GetPointer(fifo.CPReadPointer);

		FPURoundMode::SaveSIMDState();
		FPURoundMode::LoadDefaultSIMDState();
		ReadDataFromFifo(uData, 32);
		OpcodeDecoder_Run(GetVideoBufferEndPtr());
		FPURoundMode::LoadSIMDState();

		//DEBUG_LOG(COMMANDPROCESSOR, "Fifo wraps to base");

		if (fifo.CPReadPointer == fifo.CPEnd)
			fifo.CPReadPointer = fifo.CPBase;
		else
			fifo.CPReadPointer += 32;

		fifo.CPReadWriteDistance -= 32;
	}
	CommandProcessor::SetCpStatus();
}

void PreProcessingFifo(bool GPReadEnabled)
{

	SCPFifoStruct &fifo = CommandProcessor::fifo;

	if (CPBaseFifoSC
		!= fifo.CPBase || CPEndFifoSC != fifo.CPEnd)
	{
		CPBaseFifoSC = fifo.CPBase;
		CPEndFifoSC = fifo.CPEnd;
		CPReadFifoSC = fifo.CPReadPointer;
	}

	if (GPReadEnabled && CPReadFifoSC != fifo.CPWritePointer)
	{

		u8 *uData = Memory::GetPointer(CPReadFifoSC);

		if (CPReadFifoSC < fifo.CPWritePointer)
		{
			ReadDataFromPreProcFifo(uData, (fifo.CPWritePointer - CPReadFifoSC));
		}
		else
		{
			ReadDataFromPreProcFifo(uData, (fifo.CPEnd - CPReadFifoSC) + 32);
			CPReadFifoSC = fifo.CPBase;
			uData = Memory::GetPointer(CPReadFifoSC);
			ReadDataFromPreProcFifo(uData, (fifo.CPWritePointer - CPReadFifoSC));
		}
		CPReadFifoSC = fifo.CPWritePointer;

		OpcodeDecoderSC_Run(GetVideoBufferEndPtrSC());
		if (g_ActiveConfig.bWaitForShaderCompilation)
			HLSLAsyncCompiler::getInstance().WaitForCompilationFinished();
	}

}


void ReadDataFromPreProcFifo(u8* _uData, u32 len)
{
	if (len == 0) return;
	if (sizeSC + len >= FIFO_SIZE)
	{
		int pos = (int)(g_VideoDataSC.GetReadPosition() - videoBufferSC);
		sizeSC -= pos;
		if (sizeSC + len > FIFO_SIZE)
		{
			PanicAlert("FIFO SC out of bounds (size = %i, len = %i at %08x)", sizeSC, len, pos);
		}
		memmove(&videoBufferSC[0], &videoBufferSC[pos], sizeSC);
		g_VideoDataSC.SetReadPosition(videoBufferSC);
	}
	// Copy new video instructions to videoBuffer for future use in rendering the new picture
	memcpy(videoBufferSC + sizeSC, _uData, len);
	sizeSC += len;
}