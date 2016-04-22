#ifndef __RENDER_H__
#define __RENDER_H__

#include <list>

#include "SDL/include/SDL.h"
#include "Module.h"
#include "p2Point.h"

struct Sprite;

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool awake(pugi::xml_node&);

	// Called before the first frame
	bool start();

	// Called each loop iteration
	bool preUpdate();
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	// Utils
	void setViewPort(const SDL_Rect& rect);
	void resetViewPort();
	iPoint screenToWorld(int x, int y) const;

	// Draw & Blit
	bool blit(const SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool drawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool drawCircle(int x1, int y1, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	bool blit(Sprite* s);

	// Set background color
	void setBackgroundColor(SDL_Color color);

	void cameraFollow(int x, int y);

public:

	SDL_Renderer*	renderer;
	SDL_Rect		camera;
	SDL_Rect		renderZone;
	SDL_Rect		viewport;
	SDL_Color		background;

	std::list<Sprite*>	sprites;

private:
	bool isInsideRenderZone(int x, int y)const;
};

#endif // __j1RENDER_H__