#include "TestingScene.h"
#include "App.h"
#include "Map.h"
#include "Input.h"
#include "EntityManager.h"
#include "Render.h"
#include "p2Log.h"
#include "Window.h"


TestingScene::TestingScene()
{
	name.assign("test_scene");
}


TestingScene::~TestingScene()
{
}

bool TestingScene::awake(pugi::xml_node& config)
{
	bool ret = true;

	entityTextureName.assign("texture/");
	entityTextureName.append(config.first_child().first_attribute().as_string());

	return ret;
}

bool TestingScene::start()
{
	bool ret = true;

	app->map->load("map_act1.tmx");


	return ret;
}

bool TestingScene::preUpdate()
{
	bool ret = true;
	
	

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

	if (app->input->getKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool TestingScene::cleanUp()
{
	bool ret = true;

	return ret;
}