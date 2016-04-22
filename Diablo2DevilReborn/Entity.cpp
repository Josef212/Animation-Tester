#include "Textures.h"
#include "Render.h"
#include "App.h"
#include "Entity.h"
#include "Input.h"

//### PASAR A ATRIBUTOOOOOOO!!!!! velocidades diferentes segun el atributo de la entidad###
#define PLAYER_SPEED 150.0f
//###################################

Entity::Entity()
{
	
}

Entity::~Entity()
{
	app->textures->unLoad(imageTexture);
}

void Entity::setPosition(uint x, uint y)
{
	position.x = x;
	position.y = y;
	//tilePos = app->map->WorldToMap(position.x, position.y);
}

void Entity::setTilePosition(uint x, uint y)
{
	tilePos.x = x;
	tilePos.y = y;
	//position = app->map->MapToWorld(tilePos.x, tilePos.y);
}

void Entity::setCollider(SDL_Rect& col)
{
	collider = col;
}

void Entity::setSection(SDL_Rect& sect)
{
	section = sect;
}

void Entity::setCollider(const SDL_Rect& col)
{
	collider = col;
}

SDL_Rect Entity::getCollider()const
{
	return collider;
}

uint Entity::getId()const
{
	return id;
}

iPoint Entity::getPosition()
{
	return position;
}

iPoint Entity::getTilePos()
{
	return tilePos;
}

SDL_Rect Entity::getSection()const
{
	return section;
}

void Entity::draw()
{
	app->render->blit(imageTexture, position.x, position.y, &section);
}

bool Entity::entityUpdate(float internDT)
{
	return true;
}

bool Entity::entityPostUpdate()
{
	draw();
	return true;
}

//MOVEMENT METHODS
void Entity::setInitVelocity(){
	finalPos = app->input->getMouseWorldPosition();

	velocity.x = finalPos.x - position.x;
	velocity.y = finalPos.y - position.y;

	velocity.SetModule(PLAYER_SPEED);
}

void Entity::setDirection()
{
	float angle = velocity.GetAngle(); // Para la direccion ya se guarda en una propiedad de entity asi que quizas no hace falta volver a guardarla en player

	if (angle < 22.5 && angle > -22.5)
		direction = RIGHT;
	else if (angle >= 22.5 && angle <= 67.5)
		direction = DOWN_RIGHT;
	else if (angle > 67.5 && angle < 112.5)
		direction = DOWN;
	else if (angle >= 112.5 && angle <= 157.5)
		direction = DOWN_LEFT;
	else if (angle > 157.5 || angle < -157.5)
		direction = LEFT;
	else if (angle >= -157.5 && angle <= -112.5)
		direction = UP_LEFT;
	else if (angle > -112.5 && angle < -67.5)
		direction = UP;
	else if (angle >= -67.5 && angle <= -22.5)
		direction = UP_RIGHT;
}

void Entity::move(float internDT)
{
	fPoint vel = velocity * internDT;

	position.x += int(vel.x);
	position.y += int(vel.y);

	movement = !isTargetReached();

	if (movement)
	{
		updateVelocity(internDT);
	}

	//StateMachine change
	else
	{
		current_input = I_STOP_MOVE;
	}

	//Debug mode
	if (app->debug)
	{
		if (movement)
		{
			app->render->drawLine(position.x, position.y, finalPos.x, finalPos.y, 0, 0, 255);

			app->render->drawLine(position.x, position.y, vel.x * 50 + position.x, vel.y * 50 + position.y, 0, 255, 255);
			//LOG("Line: (%d, %d)", position.x, position.y);
		}
	}

}

bool Entity::isTargetReached()
{
	fPoint vel;

	vel.x = finalPos.x - position.x;
	vel.y = finalPos.y - position.y;

	if (vel.GetModule() <= finalPosRadius)
	{
		return true;
	}

	return false;
}

void Entity::updateVelocity(float internDT)
{
	velocity.x = finalPos.x - position.x;
	velocity.y = finalPos.y - position.y;

	velocity.SetModule(PLAYER_SPEED);
}