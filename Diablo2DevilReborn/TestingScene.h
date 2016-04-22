#ifndef __TESTINGSCENE_H__
#define __TESTINGSCENE_H__

#include "Module.h"
#include "Player.h"

class TestingScene :
	public Module
{
public:
	TestingScene();

	virtual ~TestingScene();

	bool awake(pugi::xml_node&);

	bool start();

	bool preUpdate();

	bool update(float dt);

	bool postUpdate();

	bool cleanUp();

private:
	UIElement* atlas = NULL;
	UIElement* staminaBar = NULL;
	UIElement* UIBase = NULL;

	/*UIElement* welcomeM;
	UIElement* rigthClickToMove;
	UIElement* rigthClickToAttack;
	UIElement* leftClickToUseHability;

	SDL_Texture* menu = NULL;
	bool blitMenu = true;
	bool moved = false;*/

	Player* player;
};

#endif