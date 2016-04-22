#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "Module.h"
#include <list>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
struct Mix_Chunk;

class Audio : public Module
{
public:
	Audio();

	virtual ~Audio();

	bool awake(pugi::xml_node&);

	bool update(float dt);

	bool cleanUp();

	bool playMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	unsigned int loadFx(const char* path);

	bool playFx(unsigned int fx, int repeat = 0);

	bool save(pugi::xml_node&)const;
	bool load(pugi::xml_node&);

private:
	_Mix_Music*				music = NULL;
	std::list<Mix_Chunk*>	fx;

public:
	int musicVolume;
};







#endif