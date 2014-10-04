// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.
// Added For Ishiiruka By Tino

#include <soundtouch/SoundTouch.h>
#include <soundtouch/STTypes.h>

#include "Core/Core.h"
#include "Core/ConfigManager.h"
#include "Core/HW/AudioInterface.h"
#include "Core/HW/SystemTimers.h"

#include "AudioCommon/AudioCommon.h"
#include "AudioCommon/SoundStream.h"
#include "AudioCommon/DPL2Decoder.h"
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
		thread = std::thread(std::mem_fn(&SoundStream::SoundLoop), this);
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
		thread.join();
	}
}

const float shortToFloat = 1.0f / 32768.0f;
#ifdef _M_ARM
inline void s16ToFloat(float* dst, const s16* src, u32 count)
{
	for (u32 i = 0; i < count; i++)
	{
		float fvalue = (float)src[i];
		fvalue = fvalue * shortToFloat;
		dst[i] = fvalue;
	}
}

inline void floatTos16(s16* dst, const float* src, u32 count)
{
	for (u32 i = 0; i < count; i++)
	{
		dst[i] = (short)(std::fminf(std::fmaxf(src[i] * 32767.0f, -32768.0f), 32767.0f));
	}
}
#else
inline void s16ToFloat(float* dst, const s16* src, u32 count)
{
	__m128 factor = _mm_set1_ps(shortToFloat);
	__m128i zero = _mm_setzero_si128();
	const __m128i* source = (__m128i*)src;
	const __m128i* end = (__m128i*)(src + count);
	while (source < end)
	{
		__m128i x = _mm_load_si128(source);
		__m128i mask = _mm_cmplt_epi16(x, zero);
		__m128i xlo = _mm_unpacklo_epi16(x, mask);
		__m128i xhi = _mm_unpackhi_epi16(x, mask);
		__m128 ylo = _mm_cvtepi32_ps(xlo);
		__m128 yhi = _mm_cvtepi32_ps(xhi);
		ylo = _mm_mul_ps(ylo, factor);
		yhi = _mm_mul_ps(yhi, factor);
		_mm_store_ps(dst, ylo);
		dst += 4;
		_mm_store_ps(dst, yhi);
		dst += 4;
		source++;
	}
}

inline void floatTos16(s16* dst, const float* src, u32 count)
{
	__m128 factor = _mm_set1_ps(32767.0f);
	__m128i* destination = (__m128i*)dst;
	__m128i* end = (__m128i*)(dst + count);
	while (destination < end)
	{
		__m128 ylo = _mm_load_ps(src);
		src += 4;
		__m128 yhi = _mm_load_ps(src);
		src += 4;
		ylo = _mm_mul_ps(ylo, factor);
		yhi = _mm_mul_ps(yhi, factor);
		__m128i xlo = _mm_cvtps_epi32(ylo);
		__m128i xhi = _mm_cvtps_epi32(yhi);
		xlo = _mm_packs_epi32(xlo, xhi);
		_mm_store_si128(destination, xlo);
		destination++;
	}
}
#endif 



GC_ALIGNED16(static short realtimeBuffer[SOUND_MAX_FRAME_SIZE]);
GC_ALIGNED16(static soundtouch::SAMPLETYPE dpl2buffer[SOUND_MAX_FRAME_SIZE]);
GC_ALIGNED16(static soundtouch::SAMPLETYPE samplebuffer[SOUND_MAX_FRAME_SIZE]);

// The audio thread.
void SoundStream::SoundLoop()
{
	Common::SetCurrentThreadName("Audio thread");
	InitializeSoundLoop();	
	bool surroundSupported = SupportSurroundOutput() && SConfig::GetInstance().m_LocalCoreStartupParameter.bDPL2Decoder;
	memset(realtimeBuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(u16));
	memset(dpl2buffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	memset(samplebuffer, 0, SOUND_MAX_FRAME_SIZE * sizeof(soundtouch::SAMPLETYPE));
	u32 channelmultiplier = surroundSupported ? SOUND_SAMPLES_SURROUND : SOUND_SAMPLES_STEREO;	
	if (SConfig::GetInstance().m_LocalCoreStartupParameter.bTimeStretching)
	{
		float ratemultiplier = 1.0f;
		soundtouch::SoundTouch sTouch;
		sTouch.setChannels(2);
		sTouch.setSampleRate(m_mixer->GetSampleRate());
		sTouch.setTempo(1.0);
		sTouch.setSetting(SETTING_SEQUENCE_MS, 35);
		sTouch.setSetting(SETTING_USE_QUICKSEEK, 0);
		sTouch.setSetting(SETTING_USE_AA_FILTER, 0);
		while (!threadData)
		{
			u32 availablesamples = m_mixer->AvailableSamples();
			u32 numsamples = availablesamples & (~(0xF));			
			if (numsamples >= 128)
			{
				float rate = m_mixer->GetCurrentSpeed();
				if (rate <= 0)
				{
					rate = 1.0f;
				}
				numsamples = m_mixer->Mix(realtimeBuffer, numsamples);				
				rate *= ratemultiplier;
				rate = rate < 0.5f ? 0.5f : rate;
				rate = roundf(rate * 32.0f) / 32.0f;
				sTouch.setTempo(rate);
				s16ToFloat(samplebuffer, realtimeBuffer, numsamples * SOUND_SAMPLES_STEREO);
				sTouch.putSamples(samplebuffer, numsamples);
			}
			u32 samplesneeded = SamplesNeeded();
			availablesamples = sTouch.numSamples();
			if (samplesneeded >= SOUND_FRAME_SIZE && availablesamples > 0)
			{
				ratemultiplier = std::fmaxf(std::fminf((float)availablesamples / (float)samplesneeded, 1.1f), 0.9f);
				numsamples = std::min(samplesneeded, SOUND_FRAME_SIZE);
				if (surroundSupported)
				{
					numsamples = sTouch.receiveSamples(dpl2buffer, numsamples);
					DPL2Decode(dpl2buffer, numsamples, samplebuffer);
				}
				else
				{
					numsamples = sTouch.receiveSamples(samplebuffer, numsamples);
				}
				floatTos16(realtimeBuffer, samplebuffer, numsamples * channelmultiplier);
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
					floatTos16(realtimeBuffer, samplebuffer, numsamples * channelmultiplier);
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