#pragma once
#include "Core.h"
#include "fmod.h"



class gPlaySoundCore
{
	enum SOUNDSTATE
	{
		BGM_NOT_LOADED,
		BGM_PLAYING,
		BGM_PAUSED,
	};

public:
	static gPlaySoundCore *GetIF();
	
	virtual ~gPlaySoundCore();

public:
	gPlaySoundCore();

	bool isBGMLoaded();

	void StartBGM(const char* filename);
	void PauseBGM();
	void ResumeBGM();
	void StopBGM();
	void PlayEffectSound(const char* filename);

	int getBGMVolume();
	void setBGMVolume(int newBGMVolume);
	int getEffectSoundVolume();
	void setEffectSoundVolume(int newEffectSoundVolume);

private:
	FSOUND_STREAM *bgmStream;	
	int bgmChannel;
	int bgmVolume;
	SOUNDSTATE bgmState;
	int effectSoundVolume;

private:
	static signed char F_CALLBACKAPI disposeEffectSoundStream(FSOUND_STREAM *stream,void *buff,int len,void *userdata);
};