#include "TestingScene.h"
#include "FadeToBlack.h"
#include "App.h"
#include "Map.h"
#include "Audio.h"
#include "Input.h"
#include "EntityManager.h"
#include "UIElement.h"
#include "UIManager.h"
#include "Render.h"
#include "Attributes.h"
#include "p2Log.h"


TestingScene::TestingScene()
{
	name.assign("scene");
}


TestingScene::~TestingScene()
{
}

bool TestingScene::awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool TestingScene::start()
{
	bool ret = true;

	//app->audio->playMusic("audio/music/Diablo_II_Act_1-_Tristram.ogg");

	app->map->load("map_act1.tmx");

	player = app->eManager->createPlayer({ 500, 1650 }, "barbarian.png", SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });

	/*staminaBar = app->uiManager->addImage({ 2850, 56, 100, 183 }, nullptr, nullptr);
	UIBase = app->uiManager->addImage({ 2872, 56, 83, 183}, nullptr, nullptr);

	staminaBar->setLocalPos(0, 300);*/
	UIBase = app->uiManager->addImage({ 923, 471, 800, 130 }, nullptr, nullptr);
	//UIBase = app->uiManager->addButton({ 50, 50, 50, 50 }, nullptr, nullptr);
	staminaBar = app->uiManager->addImage({ 736, 812, 128, 23 }, nullptr, UIBase);

	UIBase->setLocalPos(0, 600 - 130);
	staminaBar->setLocalPos(219, 600 - 130 + 94);
	/*
	welcomeM = app->uiManager->addImage({ 719, 906, 410, 157 }, this, NULL);
	welcomeM->setLocalPos(0, 0);

	rigthClickToMove = app->uiManager->addImage({ 558, 977, 112, 86 }, this, NULL);
	rigthClickToMove->setLocalPos(0, 158);

	rigthClickToAttack = app->uiManager->addImage({ 400, 977, 112, 86 }, this, NULL);
	rigthClickToAttack->setLocalPos(300000, 0);

	leftClickToUseHability = app->uiManager->addImage({ 1470, 975, 112, 86 }, this, NULL);
	leftClickToUseHability->setLocalPos(3000000, 0);*/

	//menu = app->textures->load("atlasMerda.png");

	return ret;
}

bool TestingScene::preUpdate()
{
	bool ret = true;
	
	/*if (app->input->getMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && moved == false)
	{
		blitMenu = false;
		moved = true;
		welcomeM->setLocalPos(30000, 0);
		rigthClickToMove->setLocalPos(30000, 0);
	}*/
	

	return ret;
}

bool TestingScene::update(float dt)
{
	bool ret = true;
	app->map->draw();
	return ret;
}

bool TestingScene::postUpdate()
{
	bool ret = true;

	/*if (blitMenu)
	{
		app->render->blit(menu, 0, 0);
	}*/
	
	return ret;
}

bool TestingScene::cleanUp()
{
	bool ret = true;



	return ret;
}