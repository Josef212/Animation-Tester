#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "Module.h"
#include "p2Defs.h"

class SDL_Window;
class SDL_Surface;

class Window : public Module
{
public:
	Window();

	virtual ~Window();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before quitting
	bool cleanUp();

	// Changae title
	void setTitle(const char* new_title);

	// Retrive window size
	void getWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint getScale() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

private:
	std::string	title;
	uint		width;
	uint		height;
	uint		scale;
};

#endif