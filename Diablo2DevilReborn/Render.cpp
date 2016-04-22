#include "p2Defs.h"
#include "p2Log.h"
#include "App.h"
#include "Window.h"
#include "Render.h"
#include "Player.h"
#include "Textures.h"

#define RENDER_MARGIN 150

Render::Render() : Module()
{
	LOG("Render: Creating.");

	name.assign("renderer");

	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{
	LOG("Render: Destroying");
}

// Called before render is available
bool Render::awake(pugi::xml_node& config)
{
	LOG("Render: Awake.");
	LOG("Create SDL rendering context");

	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = app->win->screen_surface->w;
		camera.h = app->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;

		renderZone.w = app->win->screen_surface->w + RENDER_MARGIN * 2;
		renderZone.h = app->win->screen_surface->h + RENDER_MARGIN * 2;
		renderZone.x = camera.x + RENDER_MARGIN;
		renderZone.y = camera.y + RENDER_MARGIN;
	}

	return ret;
}

// Called before the first frame
bool Render::start()
{
	LOG("Render: Start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::preUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::postUpdate()
{
	//Order sprites by its y position
	sprites.sort([](const Sprite* a, const Sprite* b) { return a->y < b->y;  });

	std::list<Sprite*>::iterator tmp = sprites.begin();

	while (tmp != sprites.end())
	{
		int pX, pY = 0;
		if ((*tmp)->pivotOffsetX < INT_MAX)
			pX = (*tmp)->pivotOffsetX;
		if ((*tmp)->pivotOffsetY < INT_MAX)
			pY = (*tmp)->pivotOffsetY;
		
		//if (isInsideRenderZone((*tmp)->x, (*tmp)->y))
			blit((*tmp)->texture, (*tmp)->x, (*tmp)->y, &(*tmp)->section, 1.0f, (*tmp)->angle, pX, pY);

		++tmp;
	}

	tmp = uiSprites.begin();

	while (tmp != uiSprites.end())
	{
		int pX, pY = 0;
		if ((*tmp)->pivotOffsetX < INT_MAX)
			pX = (*tmp)->pivotOffsetX;
		if ((*tmp)->pivotOffsetY < INT_MAX)
			pY = (*tmp)->pivotOffsetY;

		//if (isInsideRenderZone((*tmp)->x, (*tmp)->y))
			blit((*tmp)->texture, (*tmp)->x - app->render->camera.x, (*tmp)->y - app->render->camera.y, &(*tmp)->section, 1.0f, (*tmp)->angle, pX, pY);

		++tmp;
	}




	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	sprites.clear();

	return true;
}

// Called before quitting
bool Render::cleanUp()
{
	LOG("Render: CleanUp");
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::setBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::setViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::resetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

iPoint Render::screenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = app->win->getScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

// Blit to screen
bool Render::blit(const SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	
	if (isInsideRenderZone(x, y))
	{
		uint scale = app->win->getScale();

		SDL_Rect rect;
		rect.x = (int)(camera.x * speed) + x * scale;
		rect.y = (int)(camera.y * speed) + y * scale;

		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, &rect.w, &rect.h);
		}

		rect.w *= scale;
		rect.h *= scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivot_x != INT_MAX && pivot_y != INT_MAX)
		{
			pivot.x = pivot_x;
			pivot.y = pivot_y;
			p = &pivot;
		}

		if (SDL_RenderCopyEx(renderer, (SDL_Texture*)texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}
	else
		//LOG("Point %d, %d isn't inside.", x, y);

	return ret;
}

bool Render::drawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::drawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::drawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->getScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);


	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	if (use_camera)
	{
		x *= scale;
		y *= scale;
		x += camera.x;
		y += camera.y;
	}

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor) * scale);
		points[i].y = (int)(y + radius * sin(i * factor) * scale);
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw circle to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::blit(Sprite* s, bool isUI)
{
	bool ret = true;

	if (s != NULL && s->texture != NULL && !isUI)
	{
		sprites.push_back(s);
	}
	else if (isUI)
	{
		uiSprites.push_back(s);
	}
	else
	{
		ret = false;
		LOG("Render: Error, invalid texture to blit.");
	}

	return ret;
}

bool Render::isInsideRenderZone(int x, int y)const //Return true is inside the render zone
{
	return ((-x <= renderZone.x && -x > (renderZone.x - renderZone.w)) && (-y <= renderZone.y && -y >= (renderZone.y - renderZone.h)));
}

void Render::cameraFollow(int x, int y)
{
	uint winW, winH = 0;
	app->win->getWindowSize(winW, winH);

	camera.x = -x + (winW / 2);
	camera.y = -y + (winH / 2);
	renderZone.x = camera.x + RENDER_MARGIN;
	renderZone.y = camera.y + RENDER_MARGIN;
}
