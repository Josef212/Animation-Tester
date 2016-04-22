#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"
#include "p2Point.h"
#include "SDL\include\SDL.h"
#include "Textures.h"
#include <map>
#include "Animation.h"

enum entityType
{
	UNKNOWN,
	DUMMY
};

enum entityDirection
{
	UP = 0,
	UP_RIGHT = 1,
	RIGHT = 2,
	DOWN_RIGHT = 3,
	DOWN = 4,
	DOWN_LEFT = 5,
	LEFT = 6,
	UP_LEFT = 7
};

class Entity
{
public:
	Entity();

	virtual ~Entity();

	virtual void setPosition(uint x, uint y);

	virtual uint getId()const;

	virtual void draw();

	virtual bool entityUpdate(float internDT);

	virtual bool entityPostUpdate();

private:

	//uchar behavoiur;

public:
	uint id;
	iPoint position;
	entityType type;
	Sprite imageSprite;

	std::map<entityDirection, Animation>* animations;

	entityDirection direction = DOWN;
};

#endif