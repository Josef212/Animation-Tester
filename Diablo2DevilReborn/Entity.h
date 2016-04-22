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
	PLAYER,
	PALADIN,
	DUMMY
};

enum entityState
{
	IDLE = 0,
	WALK = 1,
	BASIC_ATTACK = 2,
	WAIT_TO_ATTACK,
	INACTIVE
};

enum entityInputState
{
	I_MOVE = 0,
	I_STOP_MOVE = 1,
	I_ATTACK = 2,
	I_NULL = 3,
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

	virtual void setTilePosition(uint x, uint y);

	virtual void setCollider(const SDL_Rect& col);

	virtual void setCollider(SDL_Rect& col);

	virtual void setSection(SDL_Rect& sect);

	virtual SDL_Rect getSection()const;

	virtual SDL_Rect getCollider()const;

	virtual uint getId()const;

	virtual iPoint getPosition();

	virtual iPoint getTilePos();

	virtual void draw();

	virtual bool entityUpdate(float internDT);

	virtual bool entityPostUpdate();

	//MOVEMENT
	iPoint		finalPos;
	fPoint		velocity;
	bool		movement = false;
	float		finalPosRadius = 2.5f;

	entityInputState current_input = I_NULL;

	void setInitVelocity();
	void setDirection();
	void move(float internDT);
	bool isTargetReached();
	void updateVelocity(float internDT);

private:

	//uchar behavoiur;

public:
	uint id;
	SDL_Rect section;
	SDL_Rect collider;
	iPoint position;
	iPoint tilePos;

	iPoint speed;
	entityType type;
	SDL_Texture* imageTexture = NULL;
	Sprite imageSprite;

	entityState state = IDLE;
	entityDirection direction;

	bool mustDie = false;

};

#endif