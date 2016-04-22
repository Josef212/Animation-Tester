#include "Textures.h"
#include "Render.h"
#include "App.h"
#include "Entity.h"
#include "Input.h"
#include "EntityManager.h"

Entity::Entity()
{
	animations = app->eManager->getAnim();
}

Entity::~Entity()
{
}

void Entity::setPosition(uint x, uint y)
{
	position.x = x;
	position.y = y;
}


uint Entity::getId()const
{
	return id;
}

void Entity::draw()
{
	SDL_Rect r;
	r = animations->find(direction)->second.getCurrentFrame();
	imageSprite.setSection(r);
	imageSprite.setPosition(position.x, position.y);
	app->render->blit(&imageSprite);
}

bool Entity::entityUpdate(float internDT)
{
		if (app->input->getKey(SDL_SCANCODE_1) == KEY_DOWN)
			direction = UP;

		if (app->input->getKey(SDL_SCANCODE_2) == KEY_DOWN)
			direction = UP_RIGHT;

		if (app->input->getKey(SDL_SCANCODE_3) == KEY_DOWN)
			direction = RIGHT;

		if (app->input->getKey(SDL_SCANCODE_4) == KEY_DOWN)
			direction = DOWN_RIGHT;

		if (app->input->getKey(SDL_SCANCODE_5) == KEY_DOWN)
			direction = DOWN;

		if (app->input->getKey(SDL_SCANCODE_6) == KEY_DOWN)
			direction = DOWN_LEFT;

		if (app->input->getKey(SDL_SCANCODE_7) == KEY_DOWN)
			direction = LEFT;

		if (app->input->getKey(SDL_SCANCODE_8) == KEY_DOWN)
			direction = UP_LEFT;

		if (app->input->getKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			position.y -= 100 * internDT;

		if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			position.y += 100 * internDT;

		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			position.x += 100 * internDT;

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			position.x -= 100 * internDT;
		

	return true;
}

bool Entity::entityPostUpdate()
{
	draw();

	app->render->cameraFollow(position.x, position.y);

	return true;
}