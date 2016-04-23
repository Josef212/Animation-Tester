#include "Textures.h"
#include "Render.h"
#include "App.h"
#include "Entity.h"
#include "Input.h"
#include "EntityManager.h"

#define PIVOT_SIZE 3

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
	r = currentAnimation->getCurrentFrame();
	app->render->blit(texture, position.x, position.y, &r);
}

void Entity::debugDraw()
{
	SDL_Rect r = currentAnimation->peekCurrentFrame();
	r.x = position.x;
	r.y = position.y;

	app->render->drawQuad(r, 255, 0, 0, 255, false, true);

	SDL_Rect pivot;
	pivot.x = currentAnimation->pivotX + position.x - 1;
	pivot.y = currentAnimation->pivotY + position.y - 1;
	pivot.w = pivot.h = PIVOT_SIZE;

	app->render->drawQuad(pivot, 0, 255, 0, 255, true, true);

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
			position.y -= 150 * internDT;

		if (app->input->getKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			position.y += 200 * internDT;

		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			position.x += 200 * internDT;

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			position.x -= 150 * internDT;

		if (app->input->getKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
			currentAnimation->reset();

	return true;
}

bool Entity::entityPostUpdate()
{
	currentAnimation = &animations->find(direction)->second;

	draw();

	app->render->cameraFollow(position.x, position.y);

	if (app->debug)
		debugDraw();

	return true;
}