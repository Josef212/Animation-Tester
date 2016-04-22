#include "Player.h"
#include "Paladin.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "Map.h"
#include "p2Log.h"
#include "Map.h"
#include "EntityManager.h"
#include "Window.h"
#include "UIManager.h"
#include "UIElement.h"

Player::Player() : Entity()
{
	LOG("Player: Creating.");
	position = { 0, 0 };
	currentPhase = BARBARIAN;
	direction = UP;
	state = IDLE;
	barbarianAnimation = app->eManager->getBarbarianAnim();
	butcherAnimation = app->eManager->getButcherAnim();
	diabloAnimation = app->eManager->getDiabloAnim();
	cooldown = 0;
	habilityDone = false;
	playerAttributes = new Attributes(100, 100, 100, 25, 10, 10, 0, 3, 50, 100);

/*	skillTree = app->uiManager->addImage({ 3419, 33, 400, 536}, nullptr, nullptr);
	skillTree->setLocalPos(800 - 400, 0);
	skillTreeActivated = app->uiManager->addImage({ 3419, 33, 400, 536 }, nullptr, nullptr);
	skillTreeActivated->setLocalPos(800 - 400, 0);
	skillTreeClicked = app->uiManager->addImage({ 3419, 33, 400, 536 }, nullptr, nullptr);
	skillTreeClicked->setLocalPos(800 - 400, 0);*/
}

Player::~Player()
{
	LOG("Entity Player: Destroying.");

	

	app->textures->unLoad(imageTexture);
}

bool Player::entityUpdate(float internDT)
{
	bool ret = true;

	app->render->cameraFollow(position.x, position.y);

	uint enemyId;
	iPoint mousePosition;

	app->input->getMousePosition(mousePosition.x, mousePosition.y);
	enemyId = app->eManager->getEntityAtPosition(mousePosition);

	Paladin* tmp = (Paladin*)app->eManager->activeEntities.find(enemyId)->second;

	if (enemyId != 0 && position.DistanceTo(tmp->position) <= tmp->getAttributes()->getRange() + 20)
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			LOG("Enemy ID: %d", attack());
			state = BASIC_ATTACK;
			
		}
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
			habilityDone = true;
		
	}
	else
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT)
		{
			setInitVelocity();
			setDirection();

			movement = true;

			//StateMachine change
			current_input = I_MOVE;
		}
	}
	if (habilityDone)
	{
		cooldown += internDT;
	}
	if (movement)
	{
		move(internDT);
		state = WALK;
	}
	else if (cooldown >= 2)
	{
		state = IDLE;
		cooldown = 0;
		habilityDone = false;
	}
	else if (!habilityDone)
	{
		state = IDLE;
	}

	if (app->input->getKey(SDL_SCANCODE_SPACE) == true)
	{
		if (playerAttributes->getHp() < playerAttributes->getHpMax())
		{
			playerAttributes->setHp(playerAttributes->getHp() + 20);
			if (playerAttributes->getHp() > playerAttributes->getHpMax())
			{
				playerAttributes->setHp(playerAttributes->getHpMax());
			}
		}
	}
	/*
	if (app->input->getMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && 
		cooldown == 0&& playerAttributes->getLevel() == 2)
	{
		app->input->getMousePosition(habilityPosition.x, habilityPosition.y);
		//habilityPosition = app->render->screenToWorld(habilityPosition.x, habilityPosition.y);
		
		//app->render->drawCircle(mousePosition.x, mousePosition.y, 30, 255, 100, 255, 255, false);
		habilityDone = true;
	}

	if (cooldown >= 6 && cooldown <= 7)
	{
		cooldown = 0;
		habilityDone = false;
	}
	else if (habilityDone)
		cooldown += internDT;
	if (habilityDone)
	{
		if (cooldown <= 1.5f)
			for (int i = 0; i < 100; i++)
			{
				app->render->drawCircle(habilityPosition.x, habilityPosition.y, i, 255, 100, 255, 255, false);
			}
			*/

	if (playerAttributes->getHp()<=0)
	{
		LOG("GAME OVER");
		//app->render->blit(&lose);
		UIImage* r = app->uiManager->addImage({ 2718, 1205, 539, 64 }, nullptr, nullptr);
		r->setLocalPos(400 - (539 / 2), 300 - (326 / 2));
		if (app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			ret = false;
	}

	playerAttributes->update();

/*
	}
	LOG("(%d,%d)", habilityPosition.x, habilityPosition.y);*/

	return ret;
}

bool Player::entityPostUpdate()
{
	bool ret = true;
	if (app->debug)
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

		if (app->input->getKey(SDL_SCANCODE_UP) == KEY_DOWN)
			state = IDLE;

		if (app->input->getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			state = WALK;

		if (app->input->getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			state = BASIC_ATTACK;

		}
	}
	

	draw();

	if (playerAttributes->getLevel() == 3)
	{
		UIImage* r = app->uiManager->addImage({ 1713, 975, 700, 289 }, nullptr, nullptr);
		r->setLocalPos(400 - (700 / 2), 300 - (289 / 2));
		if (app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			ret = false;
	}

	return ret;
}

void Player::draw()
{
	SDL_Rect r;
	switch (currentPhase)
	{
	case BARBARIAN:
		r = barbarianAnimation->find({ state, direction })->second.getCurrentFrame();
		break;
	case BUTCHER:
		r = butcherAnimation->find({ state, direction })->second.getCurrentFrame();
		break;
	case DIABLO:
		r = diabloAnimation->find({ state, direction })->second.getCurrentFrame();
		break;
	}
	//TODO will be able to remove the commentary when animations are ready

	imageSprite.setPosition(position.x - r.w/2, position.y - r.h/2);
	imageSprite.setSection(r);

	app->render->blit(&imageSprite);

	if (app->debug)
	{
		drawDebug();
	}
}

void Player::drawDebug()
{
	//app->render->drawCircle(playerAbsolutePosition.x, playerAbsolutePosition.y, 25, 255, 0, 0);
	p2Point<int> pos;
	uint w;
	uint h;
	app->win->getWindowSize(w, h);

	pos.x = w/2;
	pos.y = h/2;
	//pos = app->map->worldToMap(pos.x, pos.y);
	
	app->render->drawCircle(position.x, position.y, playerAttributes->getRange(), 255, 0, 0);
	//LOG("Circle: (%d, %d)", position.x, position.y);	
}

iPoint Player::getTilePosition() const
{
	iPoint ret = getMapPosition();
	ret = app->map->mapToWorld(ret.x, ret.y);
	ret.x -= app->map->data.tile_width / 2;
	return ret;
}

iPoint Player::getMapPosition()const
{
	return app->map->worldToMap(position.x, position.y);
}

uint Player::attack()
{
	uint ret;
	iPoint mousePosition;

	app->input->getMousePosition(mousePosition.x, mousePosition.y);
	ret = app->eManager->getEntityAtPosition(mousePosition);

	if (ret != 0)
	{
		Paladin* tmp = (Paladin*)app->eManager->activeEntities.find(ret)->second;
		if(position.DistanceTo(tmp->position) <= playerAttributes->getRange()+20 )
		{
			tmp->getAttributes()->setHp(tmp->getAttributes()->getHp() - playerAttributes->getDamage());
		}
	}
	
	

	return ret;

}