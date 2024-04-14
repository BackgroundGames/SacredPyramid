#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;
#include <vector>

using namespace std;

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	int PlayFx(unsigned int fx, int repeat = 0);

	void ChangeMusicVolume(float percent);

	void ChangeFxVolume(float percent);

	void UnloadFx(unsigned int fx);

	void StopFx(unsigned int channel);

private:

	_Mix_Music* music;
	vector<Mix_Chunk *>	fx;
};

#endif // __AUDIO_H__