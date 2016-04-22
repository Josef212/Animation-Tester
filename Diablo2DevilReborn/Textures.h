#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"
#include "p2Defs.h"
#include "SDL\include\SDL.h"

struct Sprite
{
	Sprite();
	Sprite(SDL_Texture* tex, int _x, int _y, SDL_Rect* sect = NULL, int _pivotOffsetX = 0, int _pivotOffsetY = 0, double _angle = 0, int _alpha = 255);

	void create(SDL_Texture* tex, int _x, int _y, SDL_Rect* sect = NULL, int _pivotOffsetX = 0, int _pivotOffsetY = 0, double _angle = 0, int _alpha = 255);

	void setPosition(int _x, int _y);
	void setAlpha(int _alpha);
	void setSection(SDL_Rect& sect);

	SDL_Texture* texture;
	int x, y;
	SDL_Rect section;

	int pivotOffsetX, pivotOffsetY;
	double angle;

private:
	int alpha;
};

class Textures : public Module
{
public:

	Textures();

	// Destructor
	virtual ~Textures();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called before quitting
	bool cleanUp();

	// Load Texture
	SDL_Texture* const	load(const char* path);
	bool				unLoad(SDL_Texture* texture);
	SDL_Texture* const	loadSurface(SDL_Surface* surface);
	void				getSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	std::list<SDL_Texture*>	textures;
};


#endif // __j1TEXTURES_H__