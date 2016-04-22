#include "EntityManager.h"
#include "Entity.h"
#include "Player.h"
#include "Paladin.h"
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
	name.assign("EntityManager");
}

EntityManager::~EntityManager()
{
	LOG("EntityManager: Destroying.");
	activeEntities.clear();
	inactiveEntities.clear();
}

bool EntityManager::awake(pugi::xml_node& config)
{
	LOG("EntityManager: Awake.");
	bool ret = true;

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

	paladinTexture = app->textures->load("Paladin_light.png");

	createPaladin({ 700, 1650 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 700, 1750 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 10, 900 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 10, 950 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 20, 850 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ -500, 1300 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ -550, 1320 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ -450, 1250 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ -1200, 1700 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ -1250, 1650 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ -1300, 1700 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 1100, 1300 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 1150, 1350 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 1050, 1220 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 2000, 1900 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 2050, 1950 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 2100, 1800 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });
	createPaladin({ 700, 2700 }, SDL_Rect{ 0, 0, 44, 93 }, SDL_Rect{ 0, 0, 44, 93 });


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

	if (player)
		ret = player->entityUpdate(dt);

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

	if (app->debug)
	{
		if (app->input->getMouseButtonDown(SDL_BUTTON_MIDDLE) == KEY_DOWN)
		{
			p2Point<int> point;
			point.x = app->input->getMousePosition()[0];
			point.y = app->input->getMousePosition()[1];
			point = app->render->screenToWorld(point.x, point.y);
			point.x = point.x - 22;
			point.y = point.y - (93 / 2);
			//point = app->map->worldToMap(point.x, point.y);

			app->eManager->createPaladin(
				point,
				SDL_Rect{ 0, 0, 44, 93 },
				SDL_Rect{ 0, 0, 44, 93 });
		}
	}

	return ret;
}

bool EntityManager::postUpdate()
{
	bool ret = true;

	if (player)
		ret = player->entityPostUpdate();

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

	for (tmp = inactiveEntities.begin(); tmp != inactiveEntities.end(); ++tmp)
		RELEASE(tmp->second);

	activeEntities.clear();
	inactiveEntities.clear();

	return ret;
}

bool EntityManager::loadAnimations()
{
	bool ret = true;

	pugi::xml_document	anim_file;
	pugi::xml_node		anim;
	char* buff;
	int size = app->fs->load("animations.xml", &buff);
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
		for (pugi::xml_node ent = anim.child("BARBARIAN"); ent != NULL; ent = ent.next_sibling())
		{
			for (pugi::xml_node action = ent.child("IDLE"); action != NULL; action = action.next_sibling())
			{
				for (pugi::xml_node dir = action.child("UP"); dir != NULL; dir = dir.next_sibling())
				{
					std::pair<entityState, entityDirection> p;
					int state = action.child("name").attribute("value").as_int();
					p.first = (entityState)state;

					int di = dir.first_child().attribute("name").as_int();
					p.second = (entityDirection)di;

					Animation anims;
					int x = dir.first_child().attribute("x").as_int();
					int y = dir.first_child().attribute("y").as_int();
					int w = dir.first_child().attribute("w").as_int();
					int h = dir.first_child().attribute("h").as_int();
					int fN = dir.first_child().attribute("frameNumber").as_int();
					int margin = dir.first_child().attribute("margin").as_int();
					bool loop = action.child("loop").attribute("value").as_bool();
					anims.setAnimation(x, y, w, h, fN, margin);
					anims.loop = loop;
					anims.speed = 0.15;

					int entity = ent.child("name").attribute("value").as_int();

					switch (entity) //TODO must add more cases for each new entity and animation map added
					{
					case 0:
						barbarianAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
						break;

					case 1:
						butcherAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
						break;

					case 2:
						diabloAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
						break;

					case 3:
						paladinAnim.insert(std::pair<std::pair<entityState, entityDirection>, Animation >(p, anims));
						break;
					}

				}
			}
		}
	}


	return ret;
}

bool EntityManager::remove(uint id)
{
	bool ret = true;

	if (activeEntities.erase(id) > 0)
	{
		Entity* tmp = getEntity(id);
		inactiveEntities.insert(std::pair<uint, Entity*>(id, tmp));
	}
	else
		ret = false;

	return ret;
}

Entity* EntityManager::getEntity(uint id)
{
	Entity* ret = NULL;

	std::map<uint, Entity*>::iterator tmp = activeEntities.find(id);

	return (tmp != activeEntities.end() ? tmp->second : NULL);

	return ret;
}

Player* EntityManager::getPlayer() const
{
	return player;
}

void EntityManager::sortEntities()
{

}

Entity* EntityManager::createEntity(iPoint pos, const char* textureName, SDL_Rect& section, SDL_Rect& collider, entityType _type)
{
	Entity* ret = NULL;

	ret = new Entity();

	ret->setPosition(pos.x, pos.y);
	ret->setSection(section);
	ret->setCollider(collider);
	ret->imageTexture = app->textures->load(textureName);
	ret->type = _type;
	ret->imageSprite.create(ret->imageTexture, ret->position.x, ret->position.y);
	ret->imageSprite.setSection(section);

	activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));

	ret->id = nextId;
	++nextId;

	return ret;
}

Player* EntityManager::createPlayer(iPoint pos, const char* textureName, SDL_Rect& section, SDL_Rect& collider)
{
	Player* ret = NULL;
	if (player == NULL)
	{
		ret = new Player();
		player = ret;

		ret->setPosition(pos.x, pos.y);
		ret->setSection(section);
		ret->setCollider(collider);
		ret->imageTexture = app->textures->load(textureName);
		ret->type = PLAYER;
		ret->imageSprite.create(ret->imageTexture, ret->position.x, ret->position.y);
		ret->imageSprite.setSection(section);

		ret->id = 0;
	}
	else
		LOG("Player already created.");

	return ret;
}

Paladin* EntityManager::createPaladin(iPoint pos, SDL_Rect& section, SDL_Rect& collider)
{
	Paladin* ret = NULL;

	ret = new Paladin();

	ret->setPosition(pos.x, pos.y);
	ret->setSection(section);
	ret->setCollider(collider);
	ret->type = PALADIN;
	ret->imageSprite.create(paladinTexture, ret->position.x, ret->position.y);
	ret->imageSprite.setSection(section);

	activeEntities.insert(std::pair<uint, Entity*>(nextId, ret));

	ret->id = nextId;
	++nextId;

	return ret;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getBarbarianAnim()
{
	return &barbarianAnim;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getButcherAnim()
{
	return &butcherAnim;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getDiabloAnim()
{
	return &diabloAnim;
}

std::map<std::pair<entityState, entityDirection>, Animation>* EntityManager::getPaladinAnimation()
{
	return &paladinAnim;
}

uint EntityManager::getEntityAtPosition(iPoint position)
{
	uint ret = NULL;

	iPoint worldPosition = app->render->screenToWorld(position.x, position.y);

	for (std::map<uint, Entity*>::iterator iterator = activeEntities.begin();
		iterator != activeEntities.end(); 
		iterator++)
	{
		if ((*iterator).second->collider.x <= worldPosition.x
			&& 
			(*iterator).second->collider.x + (*iterator).second->collider.w >= worldPosition.x
			&&
			(*iterator).second->collider.y <= worldPosition.y
			&&
			(*iterator).second->collider.y + (*iterator).second->collider.h >= worldPosition.y)
		{
			ret = (*iterator).first;
		}
	}

	return ret;
}
