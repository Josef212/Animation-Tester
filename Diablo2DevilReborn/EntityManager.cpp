#include "EntityManager.h"
#include "Entity.h"
#include "FileSystem.h"

#include "App.h"
#include "Render.h"
//#include "PathFinding.h"
#include "p2Log.h"
#include "Textures.h"
#include "Input.h"

EntityManager::EntityManager() : Module()
{
	LOG("EntityManager: Creating.");
	name.assign("entity_manager");
}

EntityManager::~EntityManager()
{
	LOG("EntityManager: Destroying.");
	activeEntities.clear();
}

bool EntityManager::awake(pugi::xml_node& config)
{
	LOG("EntityManager: Awake.");
	bool ret = true;

	animationName.assign("animation/");
	animationName.append(config.first_child().first_attribute().as_string());

	return ret;
}

bool EntityManager::start()
{
	LOG("EntityManager: Start.");
	bool ret = true;

	doLogic = false;
	updateMsCycle = 1 / 50;
	acummulatedTime = 0.0f;

	ret = loadAnimations();

	return ret;
}

bool EntityManager::preUpdate()
{
	bool ret = true;

	return ret;
}

bool EntityManager::update(float dt)
{
	bool ret = true;

	//TODO: limitador de fps logics
	acummulatedTime += dt;
	if (acummulatedTime >= updateMsCycle)
		doLogic = true;

	std::map<uint, Entity*>::iterator tmp = activeEntities.begin();

	for (; tmp != activeEntities.end(); ++tmp)
	{
		if (doLogic)
		{
			tmp->second->entityUpdate(dt);
			acummulatedTime = 0.0f;
		}
	}

	return ret;
}

bool EntityManager::postUpdate()
{
	bool ret = true;

	std::map<uint, Entity*>::iterator tmp = activeEntities.begin();

	for (; tmp != activeEntities.end(); ++tmp)
		tmp->second->entityPostUpdate();

	return ret;
}

bool EntityManager::cleanUp()
{
	LOG("EntityManager: CleanUp.");
	bool ret = true;

	std::map<uint, Entity*>::iterator tmp = activeEntities.begin();

	for (; tmp != activeEntities.end(); ++tmp)
		RELEASE(tmp->second);

	activeEntities.clear();

	return ret;
}

bool EntityManager::loadAnimations()
{
	bool ret = true;

	pugi::xml_document	anim_file;
	pugi::xml_node		anim;
	char* buff;
	int size = app->fs->load(animationName.c_str(), &buff);
	pugi::xml_parse_result result = anim_file.load_buffer(buff, size);
	RELEASE(buff);

	if (result == NULL)
	{
		LOG("Could not load animation xml file. Pugi error: %s", result.description());
		ret = false;
	}
	else
		anim = anim_file.child("animations");

	if (ret == true)
	{

		for (pugi::xml_node dir = anim.first_child(); dir != NULL; dir = dir.next_sibling())
		{
			entityDirection p;

			int di = dir.first_child().attribute("name").as_int();
			p = (entityDirection)di;

			Animation anims;
			int x = dir.first_child().attribute("x").as_int();
			int y = dir.first_child().attribute("y").as_int();
			int w = dir.first_child().attribute("w").as_int();
			int h = dir.first_child().attribute("h").as_int();
			int fN = dir.first_child().attribute("frameNumber").as_int();
			int margin = dir.first_child().attribute("margin").as_int();
			bool loop = anim.child("loop").attribute("value").as_bool();
			int pivotX = dir.first_child().attribute("pivot_x").as_int();
			int pivotY = dir.first_child().attribute("pivot_y").as_int();
			float speed = anim.child("speed").attribute("value").as_float();
			anims.setAnimation(x, y, w, h, fN, margin);
			anims.loop = loop;
			anims.speed = speed;
			anims.pivotX = pivotX;
			anims.pivotY = pivotY;

			animations.insert(std::pair<entityDirection, Animation>(p, anims));
		}

	}


	return ret;
}

Entity* EntityManager::getEntity(uint id)
{
	Entity* ret = NULL;

	std::map<uint, Entity*>::iterator tmp = activeEntities.find(id);

	return (tmp != activeEntities.end() ? tmp->second : NULL);

	return ret;
}

Entity* EntityManager::createEntity(iPoint pos, const char* textureName)
{
	Entity* ret = NULL;

	ret = new Entity();

	ret->setPosition(pos.x, pos.y);
	SDL_Texture* texture = app->textures->load(textureName);
	ret->texture = texture;
	ret->imageSprite.texture = texture;
	ret->type = DUMMY;

	activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));

	ret->id = nextId;
	++nextId;

	return ret;
}

std::map<entityDirection, Animation>* EntityManager::getAnim()
{
	return &animations;
}