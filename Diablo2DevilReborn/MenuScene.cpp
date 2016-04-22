#include "MenuScene.h"
#include "FadeToBlack.h"
#include "App.h"
#include "Render.h"
#include "Input.h"
#include "UIManager.h"
#include "UIElement.h"


MenuScene::MenuScene()
{
	name.assign("menuScene");
}


MenuScene::~MenuScene()
{
}

bool MenuScene::awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

bool MenuScene::start()
{
	bool ret = true;
	menuBase = app->uiManager->addImage({ 0, 0, 799, 599 }, nullptr, nullptr);
	singlePlayer = app->uiManager->addImage({ 2307, 799, 271, 34 }, nullptr, menuBase);
	singlePlayer->setLocalPos(264, 288);
	singlePlayer->setListener(this);
	singlePlayer->interactive = true;
	singlePlayer->focusable = true;
	battleNet = app->uiManager->addImage({ 2307, 799, 271, 34 }, nullptr, menuBase);
	battleNet->setLocalPos(264,331);
	cinematics = app->uiManager->addImage({ 2595, 808, 135, 23 }, nullptr, menuBase);
	cinematics->setLocalPos(402, 503);
	credits = app->uiManager->addImage({ 2748, 808, 135, 25 }, nullptr, menuBase);
	credits->setLocalPos(264, 503);
	multiplayerOptions = app->uiManager->addImage({ 2901, 799, 272, 35 }, nullptr, menuBase);
	multiplayerOptions->setLocalPos(263,398);
	gateway = app->uiManager->addImage({ 3192, 807, 272, 25 }, nullptr, menuBase);
	gateway->setLocalPos(264, 364);
	exitDiablo = app->uiManager->addImage({ 3494, 798, 272, 35 }, nullptr, menuBase);
	exitDiablo->setLocalPos(264, 533);
	return ret;
}

bool MenuScene::preUpdate()
{
	bool ret = true;
	if (app->input->getKey(SDL_SCANCODE_0) == true)
	{
		app->fade->fadeToBlack(this, (Module*)app->tS, 0.0f);
	}
	return ret;
}

bool MenuScene::update(float dt)
{
	bool ret = true;

	return ret;
}

bool MenuScene::postUpdate()
{
	bool ret = true;

	return ret;
}

bool MenuScene::cleanUp()
{
	bool ret = true;

	return ret;
}

void MenuScene::OnGui(UIElement* ui, UIEvents event)
{


	if (ui == singlePlayer)
	{
		switch (event)
		{
		case UIEvents::MOUSEIN:
		case UIEvents::MOUSELEFTDOWN:
			app->fade->fadeToBlack(this, (Module*)app->tS, 0.0f);
			break;
		}
	}
}