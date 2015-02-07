#ifndef _AUDIO_H
#define _AUDIO_H
#define WIN32_LEAN_AND_MEAN

#include <xact3.h>
#include "const.h"

namespace audioNS{
	const char WAVE_BANK[] = "audio\\wave.xwb";
	const char SOUND_BANK[] = "audio\\sound.xsb";

	const char BLAST1[] = "blast_1";
	const char BLAST2[] = "blast_2";
	const char WARP[] = "warp";
	const char THROW[] = "throw";
	const char PUT_WOOD[] = "put_light";
	const char PUT_STEEL[] = "put_middle";
	const char PUT_LEAD[] = "put_heavy";
	const char PUT_DEBU[] = "put_debu";
	const char KNOCK[] = "knock";
	const char BRAKE_WOOD[] = "brake_wood";
	const char BRAKE_STEEL[] = "brake_steel";
	const char GOAL[] = "goal";
	const char SELECT[] = "select";
	const char OK[] = "ok";
	const char CANCEL[] = "cancel";
	const char EAT_1[] = "eat_1";
	const char EAT_2[] = "eat_2";
}

class Audio {
private:
	IXACT3Engine* xact;
	IXACT3WaveBank* wave;
	IXACT3SoundBank* sound;
	XACTINDEX cueIndex;
	void* map;
	void* data;
	bool initialized;
public:
	Audio();

	virtual ~Audio();

	HRESULT initialize();

	void run();

	void playCue(const char cue[]);

	void stopCue(const char cue[]);
};

#endif