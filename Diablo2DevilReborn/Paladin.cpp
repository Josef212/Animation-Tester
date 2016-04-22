#include "Paladin.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "p2Log.h"
#include "EntityManager.h"
#include "Input.h"
#include "Player.h"

//### PASAR A ATRIBUTOOOOOOO!!!!! velocidades diferentes segun el atributo de la entidad###
#define PALA_SPEED 100.0f
//###################################

Paladin::Paladin() : Entity()
{
	speed.create(0, 0);
	animation = app->eManager->getPaladinAnimation();

	paladinAttributes = new Attributes(100, 100, 100, 10, 10, 10, 0, 3, 40, 100);
	visionRange = 200;

}

Paladin::~Paladin()
{
	app->textures->unLoad(imageTexture);
}

bool Paladin::entityUpdate(float internDT)
{
	bool ret = true;
	if (!mustDie){
		if (position.DistanceTo(app->eManager->getPlayer()->position) < visionRange)
		{
			//LOG("I see the player!!!");
			if (position.DistanceTo(app->eManager->getPlayer()->position) > paladinAttributes->getRange() + 20 || position.DistanceTo(app->eManager->getPlayer()->position) < paladinAttributes->getRange() - 20)
			{
				setInitVelocity();
				setDirection();

				movement = true;

				//StateMachine change
				current_input = I_MOVE;
			}
			if (movement) //&& (position.DistanceTo(app->eManager->getPlayer()->position) > paladinAttributes->getRange() + 15))
			{
				move(internDT);
				state = WALK;
			}
			else
			{
				setAttackDirection();
				state = BASIC_ATTACK;

				if (lastAttack >= 1){
					app->eManager->getPlayer()->playerAttributes->setHp(app->eManager->getPlayer()->playerAttributes->getHp() - paladinAttributes->getDamage());
					lastAttack = 0.0f;
				}
				lastAttack += internDT;
			}
		}
		else
		{
			state = IDLE;
		}

		collider.x = position.x - collider.w / 2;
		collider.y = position.y - collider.h / 2;

		if (paladinAttributes->getHp() <= 0)
		{
			LOG("I'm dead <3");
			mustDie = true;
			app->eManager->getPlayer()->playerAttributes->gainExp(paladinAttributes->getDropExp());
		}

		if (app->debug)
		{
			drawDebug();
		}

		
	}

	return ret;
}

bool Paladin::entityPostUpdate()
{
	bool ret = true;

	if (!mustDie)
	{
		draw();
	}
	
		return ret;
}

void Paladin::setInitVelocity(){
	finalPos = app->eManager->getPlayer()->position;

	velocity.x = finalPos.x - position.x;
	velocity.y = finalPos.y - position.y;

	velocity.SetModule(PALA_SPEED);

	float angle = velocity.GetAngle();
	float escalar = 0.02;

	if (angle < 22.5 && angle > -22.5)
		finalPos.x = finalPos.x - paladinAttributes->getRange() + (angle * escalar); //RIGHT
	else if (angle >= 22.5 && angle <= 67.5)
	{
		finalPos.x = finalPos.x - paladinAttributes->getRange() + (angle * escalar); //DOWN_RIGHT
		finalPos.y = finalPos.y - paladinAttributes->getRange() + (angle * escalar);
	}
	else if (angle > 67.5 && angle < 112.5)
		finalPos.y = finalPos.y - paladinAttributes->getRange() + (angle * escalar); //DOWN
	else if (angle >= 112.5 && angle <= 157.5)
	{
		finalPos.x = finalPos.x + paladinAttributes->getRange() + (angle * escalar);//DOWN_LEFT
		finalPos.y = finalPos.y - paladinAttributes->getRange() + (angle * escalar);
	}
	else if (angle > 157.5 || angle < -157.5)
		finalPos.x = finalPos.x + paladinAttributes->getRange() + (angle * escalar);//LEFT
	else if (angle >= -157.5 && angle <= -112.5)
	{
		finalPos.x = finalPos.x + paladinAttributes->getRange() + (angle * escalar);//UP_LEFT
		finalPos.y = finalPos.y + paladinAttributes->getRange() + (angle * escalar);
	}
	else if (angle > -112.5 && angle < -67.5)
		finalPos.y = finalPos.y + paladinAttributes->getRange() + (angle * escalar);//UP
	else if (angle >= -67.5 && angle <= -22.5)
	{
		finalPos.x = finalPos.x - paladinAttributes->getRange() + (angle * escalar);//UP_RIGHT
		finalPos.y = finalPos.y + paladinAttributes->getRange() + (angle * escalar);
	}

	velocity.x = finalPos.x - position.x;
	velocity.y = finalPos.y - position.y;

	velocity.SetModule(PALA_SPEED);
}

void Paladin::updateVelocity(float internDT)
{
	velocity.x = finalPos.x - position.x;
	velocity.y = finalPos.y - position.y;

	velocity.SetModule(PALA_SPEED);
}

void Paladin::setAttackDirection()
{
	finalPos = app->eManager->getPlayer()->position;

	velocity.x = finalPos.x - position.x;
	velocity.y = finalPos.y - position.y;

	velocity.SetModule(PALA_SPEED);

	setDirection();
}

void Paladin::draw()
{
	SDL_Rect r = animation->find({ state, direction })->second.getCurrentFrame();

	imageSprite.setPosition(position.x - r.w/2, position.y - r.h/2);
	imageSprite.setSection(r);

	app->render->blit(&imageSprite);
}

void Paladin::drawDebug()
{
	app->render->drawCircle(position.x, position.y, paladinAttributes->getRange(), 255, 0, 0);
	app->render->drawCircle(position.x, position.y, visionRange, 255, 100, 200);
	app->render->drawQuad(collider, 0, 255, 0, 255, false);
	//LOG("Circle: (%d, %d)", position.x, position.y);

}

Attributes* Paladin::getAttributes()
{
	return paladinAttributes;
}