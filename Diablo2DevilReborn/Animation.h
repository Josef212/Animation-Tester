#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <vector>
#include "SDL\include\SDL.h"

class Animation
{

public:

	float speed;
	bool loop;
	std::vector<SDL_Rect> frames;

private:

	float currentFrame;
	int loops;

public:

	Animation() : speed(1.0f), currentFrame(0), loop(true), loops(0)
	{}

	Animation(const Animation& a) : frames(a.frames), speed(a.speed), currentFrame(0), loop(a.loop), loops(0)
	{}

	SDL_Rect& getCurrentFrame()
	{
		currentFrame += speed;
		if (currentFrame >= frames.size())
		{
			currentFrame = (loop) ? 0.0f : frames.size() - 1;
			loops++;
		}

		return frames[(int)currentFrame];

	}

	const SDL_Rect& peekCurrentFrame() const
	{
		return frames[(int)currentFrame];
	}

	bool finished()
	{
		return loops > 0;
	}

	void reset()
	{
		currentFrame = 0.0f;
	}

	void resetLoops()
	{
		loops = 0;
	}

	// ya lo revisare mañana
	/*bool isOver()
	{
		return (&frames[((int)currentFrame + 1)] == NULL && loop == false);
	}*/
	
	void setAnimation(int x, int y, int w, int h, int frameNum, int margin)
	{
		for (int i = 0; i < frameNum; ++i)
		{
			frames.push_back({ x + (w + margin) * i, y, w, h });
		}
	}

};



#endif
