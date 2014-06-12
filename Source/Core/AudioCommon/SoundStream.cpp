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

// The audio thread.
void SoundStream::SoundLoop()
{
	Common::SetCurrentThreadName("Audio thread");
	InitializeSoundLoop();
	m_soundTouch.setChannels(2);
	m_soundTouch.setSampleRate(m_mixer->GetSampleRate());
	m_soundTouch.setTempo(1.0);
	m_soundTouch.setSetting(SETTING_USE_QUICKSEEK, 0);
	m_soundTouch.setSetting(SETTING_USE_AA_FILTER, 0);
	bool surroundSupported = SupportSurroundOutput() && Core::g_CoreStartupParameter.bDPL2Decoder;
	GC_ALIGNED16(short realtimeBuffer[SOUND_MAX_FRAME_SIZE]);
	memset(realtimeBuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(u16));
	GC_ALIGNED16(soundtouch::SAMPLETYPE dpl2buffer[SOUND_MAX_FRAME_SIZE]);
	memset(dpl2buffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	GC_ALIGNED16(soundtouch::SAMPLETYPE samplebuffer[SOUND_MAX_FRAME_SIZE]);
	memset(samplebuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	const float shortToFloat = 1.0f / 32768.0f;
	float ratemultiplier = 1.0f;
	s32 channelmultiplier = surroundSupported ? SOUND_SAMPLES_SURROUND : SOUND_SAMPLES_STEREO;
	while (!threadData)
	{
		int numsamples = m_mixer->AvailableSamples();
		if (numsamples > 128)
		{
			numsamples = m_mixer->Mix(realtimeBuffer, numsamples);
			float rate = m_mixer->GetCurrentSpeed();
			if (rate <= 0)
			{
				rate = 1.0f;
			}
			rate *= ratemultiplier;
			rate = rate < 0.6f ? 0.6f : rate;
			rate = roundf(rate * 32.0f) / 32.0f;
			m_soundTouch.setTempo(rate);
			for (s32 i = 0; i < numsamples * SOUND_SAMPLES_STEREO; i++)
			{
				float fvalue = (float)realtimeBuffer[i];
				fvalue = fvalue * shortToFloat;
				samplebuffer[i] = fvalue;
			}
			m_soundTouch.putSamples(samplebuffer, numsamples);
		}
		numsamples = SamplesNeeded();
		u32 availablesamples = m_soundTouch.numSamples();
		ratemultiplier = std::fmaxf(std::fminf((float)availablesamples / (numsamples * 1.2f), 2.0f), 0.5f);
		if (numsamples >= SOUND_FRAME_SIZE && availablesamples > 0)
		{
			numsamples = std::min(numsamples, SOUND_FRAME_SIZE);
			if (surroundSupported)
			{
				numsamples = m_soundTouch.receiveSamples(dpl2buffer, numsamples);
				dpl2decode(dpl2buffer, numsamples, samplebuffer);
			}
			else
			{
				numsamples = m_soundTouch.receiveSamples(samplebuffer, numsamples);
			}			
			for (s32 i = 0; i < numsamples * channelmultiplier; i++)
			{
				realtimeBuffer[i] = (short)(samplebuffer[i] * 32767.0f);
			}
			WriteSamples(realtimeBuffer, numsamples);
		}
		Common::YieldCPU();
	}
}