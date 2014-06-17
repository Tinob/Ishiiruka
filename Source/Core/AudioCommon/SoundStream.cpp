// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.


#include "AudioCommon/AudioCommon.h"
#include "AudioCommon/SoundStream.h"
#include "AudioCommon/DPL2Decoder.h"
#include "Common/StdThread.h"
#include "Common/Thread.h"
#include "Common/Event.h"
#include "Core/Core.h"
#include "Core/HW/AudioInterface.h"
#include "Core/HW/SystemTimers.h"
#include <soundtouch/SoundTouch.h>
#include <soundtouch/STTypes.h>

soundtouch::SoundTouch m_soundTouch;

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
		
		dpl2reset();
		thread = std::thread(std::mem_fn(&SoundStream::SoundLoop), this);
	}
	return true;
}

void SoundStream::Clear(bool mute)
{
	m_muted = mute;
	m_soundTouch.clear();	
}

void SoundStream::Stop()
{
	threadData = 1;
	thread.join();
	m_soundTouch.clear();
}

const float shortToFloat = 1.0f / 32768.0f;

inline void s16ToFloat(float* dst, const s16* src, s32 count)
{
	for (s32 i = 0; i < count; i++)
	{
		float fvalue = (float)src[i];
		fvalue = fvalue * shortToFloat;
		dst[i] = fvalue;
	}
}

inline void floatTos16(s16* dst, const float* src, s32 count)
{
	for (s32 i = 0; i < count; i++)
	{
		dst[i] = (short)(src[i] * 32767.0f);
	}
}

// The audio thread.
void SoundStream::SoundLoop()
{
	Common::SetCurrentThreadName("Audio thread");
	InitializeSoundLoop();	
	bool surroundSupported = SupportSurroundOutput() && Core::g_CoreStartupParameter.bDPL2Decoder;
	GC_ALIGNED16(short realtimeBuffer[SOUND_MAX_FRAME_SIZE]);
	memset(realtimeBuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(u16));
	GC_ALIGNED16(soundtouch::SAMPLETYPE dpl2buffer[SOUND_MAX_FRAME_SIZE]);
	memset(dpl2buffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	GC_ALIGNED16(soundtouch::SAMPLETYPE samplebuffer[SOUND_MAX_FRAME_SIZE]);
	memset(samplebuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	s32 channelmultiplier = surroundSupported ? SOUND_SAMPLES_SURROUND : SOUND_SAMPLES_STEREO;	
	if (Core::g_CoreStartupParameter.bTimeStretching)
	{
		float ratemultiplier = 1.0f;
		m_soundTouch.setChannels(2);
		m_soundTouch.setSampleRate(m_mixer->GetSampleRate());
		m_soundTouch.setTempo(1.0);
		m_soundTouch.setSetting(SETTING_USE_QUICKSEEK, 0);
		m_soundTouch.setSetting(SETTING_USE_AA_FILTER, 1);
		while (!threadData)
		{
			int numsamples = m_mixer->AvailableSamples();
			if (numsamples > 128)
			{
				float rate = m_mixer->GetCurrentSpeed();
				if (rate <= 0)
				{
					rate = 1.0f;
				}
				numsamples = m_mixer->Mix(realtimeBuffer, numsamples);				
				rate *= ratemultiplier;
				rate = rate < 0.6f ? 0.6f : rate;
				rate = roundf(rate * 32.0f) / 32.0f;
				m_soundTouch.setTempo(rate);
				s16ToFloat(samplebuffer, realtimeBuffer, numsamples * SOUND_SAMPLES_STEREO);
				m_soundTouch.putSamples(samplebuffer, numsamples);
			}
			s32 samplesneeded = SamplesNeeded();
			s32 availablesamples = m_soundTouch.numSamples();
			if (samplesneeded >= SOUND_FRAME_SIZE && availablesamples > 0)
			{
				ratemultiplier = std::fmaxf(std::fminf((float)availablesamples / (samplesneeded * 1.2f), 2.0f), 0.5f);
				numsamples = std::min(samplesneeded, SOUND_FRAME_SIZE);
				if (surroundSupported)
				{
					numsamples = m_soundTouch.receiveSamples(dpl2buffer, numsamples);
					dpl2decode(dpl2buffer, numsamples, samplebuffer);
				}
				else
				{
					numsamples = m_soundTouch.receiveSamples(samplebuffer, numsamples);
				}
				floatTos16(realtimeBuffer, samplebuffer, numsamples * channelmultiplier);
				WriteSamples(realtimeBuffer, numsamples);
				samplesneeded -= numsamples;
			}
			else 
			{
				Common::YieldCPU();
			}
		}
	}
	else
	{
		while (!threadData)
		{
			s32 neededsamples = SamplesNeeded();
			if (neededsamples >= SOUND_FRAME_SIZE)
			{
				neededsamples = std::min(neededsamples, SOUND_FRAME_SIZE);
				s32 availablesamples = m_mixer->AvailableSamples();
				s32 numsamples = std::min(neededsamples, availablesamples);
				numsamples = m_mixer->Mix(realtimeBuffer, numsamples);
				if (surroundSupported)
				{
					s16ToFloat(dpl2buffer, realtimeBuffer, numsamples * SOUND_SAMPLES_STEREO);
					dpl2decode(dpl2buffer, numsamples, samplebuffer);
					floatTos16(realtimeBuffer, samplebuffer, numsamples * channelmultiplier);
				}
				WriteSamples(realtimeBuffer, numsamples);
			}
			else
			{
				Common::YieldCPU();
			}
		}
	}
}