// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added For Ishiiruka By Tino
#ifdef _WIN32
#include <windows.h>
#endif
#ifdef __APPLE__
// Avoid conflict with objc.h (on Windows, ST uses the system BOOL type, so this doesn't work)
#define BOOL SoundTouch_BOOL
#endif
#include <soundtouch/SoundTouch.h>
#include <soundtouch/STTypes.h>
#ifdef __APPLE__
#undef BOOL
#endif
#include "Core/Core.h"
#include "Core/ConfigManager.h"
#include "Core/HW/AudioInterface.h"
#include "Core/HW/SystemTimers.h"

#include "AudioCommon/AudioCommon.h"
#include "AudioCommon/SoundStream.h"
#include "AudioCommon/DPL2Decoder.h"

#include "Common/MathUtil.h"
#include "Common/StdThread.h"


SoundStream::SoundStream(CMixer *mixer) : m_mixer(mixer), threadData(0), m_logAudio(false), m_muted(false), m_enablesoundloop(true)
{

}

SoundStream::~SoundStream() 
{
	delete m_mixer; 
}

void SoundStream::StartLogAudio(const char *filename) {
	if (!m_logAudio) {
		m_logAudio = true;
		g_wave_writer.Start(filename, m_mixer->GetSampleRate());
		g_wave_writer.SetSkipSilence(false);
		NOTICE_LOG(DSPHLE, "Starting Audio logging");
	}
	else {
		WARN_LOG(DSPHLE, "Audio logging already started");
	}
}

void SoundStream::StopLogAudio() {
	if (m_logAudio) {
		m_logAudio = false;
		g_wave_writer.Stop();
		NOTICE_LOG(DSPHLE, "Stopping Audio logging");
	}
	else {
		WARN_LOG(DSPHLE, "Audio logging already stopped");
	}
}
bool SoundStream::Start()
{ 
	if (m_enablesoundloop)
	{
		DPL2Reset();
		thread.reset(new std::thread(std::mem_fn(&SoundStream::SoundLoop), this));
#ifdef _WIN32
		SetThreadPriority(thread.get()->native_handle(), THREAD_PRIORITY_HIGHEST);
#endif
	}
	return true;
}

void SoundStream::Clear(bool mute)
{
	m_muted = mute;
}

void SoundStream::Stop()
{
	if (m_enablesoundloop)
	{
		threadData = 1;
		thread.get()->join();
	}
}

GC_ALIGNED16(static short realtimeBuffer[SOUND_MAX_FRAME_SIZE]);
GC_ALIGNED16(static soundtouch::SAMPLETYPE dpl2buffer[SOUND_MAX_FRAME_SIZE]);
GC_ALIGNED16(static soundtouch::SAMPLETYPE samplebuffer[SOUND_MAX_FRAME_SIZE]);

float s_dither_prev[SOUND_SAMPLES_SURROUND];

__forceinline void floatTos16(s16* dst, const float *src, u32 numsamples, u32 numchannels)
{
	for (u32 i = 0; i < numsamples; i++)
	{
		for (u32 j = 0; j < numchannels; j++)
		{
			float sample = src[i * numchannels + j] * 32768.0f;
			TriangleDither(sample, s_dither_prev[j]);
			MathUtil::Clamp(&sample, -32768.f, 32767.f);
			dst[i * numchannels + j] = s16(sample);
		}
	}
}

__forceinline void s16ToFloat(float* dst, const s16 *src, u32 numsamples)
{
	for (u32 i = 0; i < numsamples; i++)
	{
		s16 sample = src[i];
		dst[i] = Signed16ToFloat(sample);
	}
}

// The audio thread.
void SoundStream::SoundLoop()
{
	Common::SetCurrentThreadName("Audio thread");
	InitializeSoundLoop();	
	bool surroundSupported = SupportSurroundOutput() && SConfig::GetInstance().m_LocalCoreStartupParameter.bDPL2Decoder;
	memset(realtimeBuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(u16));
	memset(dpl2buffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	memset(samplebuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	memset(s_dither_prev, 0, sizeof(s_dither_prev));
	u32 channelmultiplier = surroundSupported ? SOUND_SAMPLES_SURROUND : SOUND_SAMPLES_STEREO;	
	if (SConfig::GetInstance().m_LocalCoreStartupParameter.bTimeStretching)
	{
		float ratemultiplier = 1.0f;
		soundtouch::SoundTouch sTouch;
		sTouch.setChannels(2);
		sTouch.setSampleRate(m_mixer->GetSampleRate());
		sTouch.setTempo(1.0);
		sTouch.setSetting(SETTING_USE_QUICKSEEK, 0);
		sTouch.setSetting(SETTING_USE_AA_FILTER, 1);
		while (!threadData)
		{
			u32 availablesamples = m_mixer->AvailableSamples();
			u32 numsamples = std::min(availablesamples, 400u);
			if (numsamples == 400u)
			{
				float rate = m_mixer->GetCurrentSpeed();
				if (rate <= 0)
				{
					rate = 1.0f;
				}
				numsamples = m_mixer->Mix(samplebuffer, numsamples);
				rate *= ratemultiplier;
				rate = rate < 0.5f ? 0.5f : rate;
				rate = roundf(rate * 16.0f) / 16.0f;
				sTouch.setTempo(rate);
				sTouch.putSamples(samplebuffer, numsamples);
			}
			u32 samplesneeded = SamplesNeeded();
			availablesamples = sTouch.numSamples();
			if (samplesneeded >= SOUND_FRAME_SIZE && availablesamples > 0)
			{
				ratemultiplier = std::fmaxf(std::fminf((float)availablesamples / (float)samplesneeded, 1.1f), 0.9f);
				numsamples = std::min(availablesamples, SOUND_FRAME_SIZE);
				if (surroundSupported)
				{
					numsamples = sTouch.receiveSamples(dpl2buffer, numsamples);
					DPL2Decode(dpl2buffer, numsamples, samplebuffer);
					// zero-out the subwoofer channel - DPL2Decode generates a pretty
					// good 5.0 but not a good 5.1 output.  Sadly there is not a 5.0
					// AL_FORMAT_50CHN32 to make this super-explicit.
					// DPL2Decode output: LEFTFRONT, RIGHTFRONT, CENTREFRONT, (sub), LEFTREAR, RIGHTREAR
					for (u32 i = 0; i < numsamples; ++i)
					{
						samplebuffer[i*SOUND_SAMPLES_SURROUND + 3 /*sub/lfe*/] = 0.0f;
					}
				}
				else
				{
					numsamples = sTouch.receiveSamples(samplebuffer, numsamples);
				}
				floatTos16(realtimeBuffer, samplebuffer, numsamples, channelmultiplier);
				WriteSamples(realtimeBuffer, numsamples);
			}
			else 
			{
				Common::SleepCurrentThread(1);
			}
		}
	}
	else
	{
		while (!threadData)
		{
			u32 neededsamples = std::min(SamplesNeeded(), SOUND_FRAME_SIZE);
			u32 availablesamples = m_mixer->AvailableSamples() & (~(0xF));
			if (neededsamples == SOUND_FRAME_SIZE && availablesamples > 0)
			{
				u32 numsamples = std::min(availablesamples, neededsamples);
				numsamples = m_mixer->Mix(realtimeBuffer, numsamples);
				if (surroundSupported)
				{
					s16ToFloat(dpl2buffer, realtimeBuffer, numsamples * SOUND_SAMPLES_STEREO);
					DPL2Decode(dpl2buffer, numsamples, samplebuffer);
					floatTos16(realtimeBuffer, samplebuffer, numsamples, channelmultiplier);
				}
				WriteSamples(realtimeBuffer, numsamples);
			}
			else
			{
				Common::SleepCurrentThread(1);
			}
		}
	}
}