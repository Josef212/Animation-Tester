#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "p2Point.h"
#include <map>

#include "SDL\include\SDL.h"

class EntityManager : public Module
{
public:
	EntityManager();

	virtual ~EntityManager();


	bool awake(pugi::xml_node&);

	bool start();

	bool preUpdate();

	bool update(float dt);

	bool postUpdate();

	bool cleanUp();

	Entity* getEntity(uint id);

	Entity* createEntity(iPoint pos, const char* textureName);

	std::map<entityDirection, Animation>* getAnim();

	std::map<uint, Entity*>		activeEntities;

private:

	uint nextId = 1;

	bool doLogic;

	float updateMsCycle; // 1/fps de la logica
	float acummulatedTime;

	std::map<entityDirection, Animation> animations;

private:

	bool loadAnimations();

	std::string animationName;
};

#endif