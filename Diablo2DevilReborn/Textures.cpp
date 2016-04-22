#include "p2Log.h"
#include "App.h"
#include "Render.h"
#include "FileSystem.h"
#include "Textures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )


Sprite::Sprite()
{
	texture = NULL;
	x = y = pivotOffsetX = pivotOffsetY = 0;
	alpha = 255;
}

Sprite::Sprite(SDL_Texture* tex, int _x, int _y, SDL_Rect* sect, int _pivotOffsetX, int _pivotOffsetY, double _angle, int _alpha)
{
	texture = tex;
	x = _x;
	y = _y;

	if (sect != NULL)
		section = *sect;
	else
		SDL_QueryTexture(texture, NULL, NULL, &section.w, &section.h);

	pivotOffsetX = _pivotOffsetX;
	pivotOffsetY = _pivotOffsetY;
	angle = _angle;
	alpha = _alpha;
	SDL_SetTextureAlphaMod(this->texture, this->alpha);
}

void Sprite::create(SDL_Texture* tex, int _x, int _y, SDL_Rect* sect, int _pivotOffsetX, int _pivotOffsetY, double _angle, int _alpha)
{
	texture = tex;
	x = _x;
	y = _y;
	
	if (sect != NULL)
		section = *sect;
	else
		SDL_QueryTexture(texture, NULL, NULL, &section.w, &section.h);

	pivotOffsetX = _pivotOffsetX;
	pivotOffsetY = _pivotOffsetY;
	angle = _angle;
	alpha = _alpha;
	SDL_SetTextureAlphaMod(this->texture, this->alpha);
}

void Sprite::setPosition(int _x, int _y)
{
	x = _x;
	y = _y;
}

void Sprite::setAlpha(int _alpha)
{
	this->alpha = _alpha;
	SDL_SetTextureAlphaMod(this->texture, this->alpha);
}

void Sprite::setSection(SDL_Rect& sect)
{
	section = sect;
}

Textures::Textures() : Module()
{
	LOG("Textures: Creating.");
	name.assign("textures");
}

// Destructor
Textures::~Textures()
{
	LOG("Textures: Destroying.");
}

// Called before render is available
bool Textures::awake(pugi::xml_node& config)
{
	LOG("Textures: Awake.");
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::start()
{
	LOG("Textures: Start.");
	bool ret = true;

	return ret;
}

// Called before quitting
bool Textures::cleanUp()
{
	LOG("Textures: CleanUp.");
	LOG("Freeing textures and Image library");
	std::list<SDL_Texture*>::iterator item;

	for (item = textures.begin(); item != textures.end(); item++)
	{
		SDL_DestroyTexture(*item);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::load(const char* path)
{
	LOG("Textures: Loading a texture.");

	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load_RW(app->fs->load(path), 1);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = loadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool Textures::unLoad(SDL_Texture* texture)
{
	std::list<SDL_Texture*>::iterator item;

	for (item = textures.begin(); item != textures.end(); item++)
	{
		if (texture == *item)
		{
			SDL_DestroyTexture(*item);
			textures.erase(item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const Textures::loadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void Textures::getSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}
