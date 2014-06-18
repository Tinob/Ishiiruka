// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include "AudioCommon/Mixer.h"
#include "AudioCommon/WaveFile.h"
#include "Common/Common.h"
#include "Common/Thread.h"


#define SOUND_FRAME_SIZE 2048u
#define SOUND_SAMPLES_STEREO 2u
#define SOUND_SAMPLES_SURROUND 6u
#define SOUND_STEREO_FRAME_SIZE (SOUND_FRAME_SIZE * SOUND_SAMPLES_STEREO)
#define SOUND_SURROUND_FRAME_SIZE (SOUND_FRAME_SIZE * SOUND_SAMPLES_SURROUND)
#define SOUND_STEREO_FRAME_SIZE_BYTES (SOUND_STEREO_FRAME_SIZE * sizeof(s16))
#define SOUND_SURROUND_FRAME_SIZE_BYTES (SOUND_SURROUND_FRAME_SIZE * sizeof(s16))

#define SOUND_MAX_FRAME_SIZE (SOUND_FRAME_SIZE * SOUND_SAMPLES_SURROUND)
#define SOUND_MAX_FRAME_SIZE_BYTES (SOUND_MAX_FRAME_SIZE * sizeof(s16))
#define SOUND_BUFFER_COUNT 6u

class SoundStream
{
protected:
	bool m_enablesoundloop;
	CMixer *m_mixer;
	// We set this to shut down the sound thread.
	// 0=keep playing, 1=stop playing NOW.
	volatile int threadData;
	bool m_logAudio;
	WaveFileWriter g_wave_writer;
	bool m_muted;
	std::thread thread;
	void SoundLoop();
	virtual void InitializeSoundLoop() {}
	virtual u32 SamplesNeeded(){ return 0; }
	virtual void WriteSamples(s16 *src, u32 numsamples){}
	virtual bool SupportSurroundOutput(){ return false; };
public:
	SoundStream(CMixer *mixer);
	~SoundStream();

	static  bool isValid() { return false; }
	virtual CMixer *GetMixer() const { return m_mixer; }
	virtual bool Start();
	virtual void SetVolume(int) {}	
	virtual void Stop();
	virtual void Clear(bool mute);
	virtual void Update() {};
	bool IsMuted() const { return m_muted; }
	virtual void StartLogAudio(const char *filename);

	virtual void StopLogAudio();
};
