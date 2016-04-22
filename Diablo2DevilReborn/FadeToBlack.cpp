#include "FadeToBlack.h"
#include "App.h"
#include "Render.h"

FadeToBlack::FadeToBlack() : Module(), counter(0.0f), maxTime(0.0f), fading(false)
{
}

bool FadeToBlack::start()
{
	screen = app->render->camera;

	return true;
}

FadeToBlack::~FadeToBlack()
{
}

bool FadeToBlack::update(float dt)
{
	bool ret = true;

	if (maxTime >= 0.0f && fading == true)
	{
		if (counter <= maxTime)
		{
			moduleOff->unInit();
			app->render->drawQuad(screen, 0, 0, 0, 255, true, false);
			counter += dt;
			//(Uint8)counter *
		}
		else
		{
			moduleOff->disable();
			moduleOn->enable();
			maxTime = 0.0f;
			fading = false;
		}
	}

	return ret;
}

void FadeToBlack::fadeToBlack(Module* mOff, Module* mOn, float time)//Time in seconds
{
	moduleOn = mOn;
	moduleOff = mOff;
	maxTime = time;
	fading = true;
}