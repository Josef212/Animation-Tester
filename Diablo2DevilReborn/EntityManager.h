#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "p2Point.h"
#include <map>

#include "SDL\include\SDL.h"

class Player;
class Paladin;

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

	Player* getPlayer() const;

	bool remove(uint _id);

	Entity* createEntity(iPoint pos, const char* textureName, SDL_Rect& section, SDL_Rect& collider, entityType _type);

	Player* createPlayer(iPoint pos, const char* textureName, SDL_Rect& section, SDL_Rect& collider);

	Paladin* createPaladin(iPoint pos, SDL_Rect& section, SDL_Rect& collider);

	std::map<std::pair<entityState, entityDirection>, Animation>* getBarbarianAnim();
	std::map<std::pair<entityState, entityDirection>, Animation>* getButcherAnim();
	std::map<std::pair<entityState, entityDirection>, Animation>* getDiabloAnim();
	std::map<std::pair<entityState, entityDirection>, Animation>* getPaladinAnimation();

	uint getEntityAtPosition(iPoint position);


	std::map<uint, Entity*>		activeEntities;
	std::map<uint, Entity*>		inactiveEntities;

private:

	uint nextId = 1;

	Player* player = NULL;

	bool doLogic;

	float updateMsCycle; // 1/fps de la logica
	float acummulatedTime;

	std::map<std::pair<entityState, entityDirection>, Animation>	barbarianAnim;
	std::map<std::pair<entityState, entityDirection>, Animation>	butcherAnim;
	std::map<std::pair<entityState, entityDirection>, Animation>	diabloAnim;
	std::map<std::pair<entityState, entityDirection>, Animation>	paladinAnim;

	SDL_Texture* paladinTexture = NULL;

private:
	void sortEntities();

	bool loadAnimations();
};

#endif