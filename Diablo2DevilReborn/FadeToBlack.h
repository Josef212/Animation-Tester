#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"
#include "SDL\include\SDL.h"

class FadeToBlack : public Module
{
public:
	FadeToBlack();
	~FadeToBlack();

	bool start();
	bool update(float dt);

	void fadeToBlack(Module* mOff, Module* mOn, float time = 2.0f);

private:
	Module* moduleOn = NULL;
	Module* moduleOff = NULL;

	bool fading;
	SDL_Rect screen;

	float counter;
	float maxTime;
};


#endif