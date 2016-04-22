#ifndef __TESTINGSCENE_H__
#define __TESTINGSCENE_H__

#include "Module.h"

class Entity;

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
	std::string entityTextureName;
	std::string mapName;
	Entity* entity = NULL;
};

#endif