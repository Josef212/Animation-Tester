#ifndef __TESTINGSCENE2_H__
#define __TESTINGSCENE2_H__

#include "Module.h"

class UIImage;

class MenuScene : public Module
{
public:
	MenuScene();

	virtual ~MenuScene();

	bool awake(pugi::xml_node&);

	bool start();

	bool preUpdate();

	bool update(float dt);

	bool postUpdate();

	bool cleanUp();

	void OnGui(UIElement* ui, UIEvents event);

private:
	UIElement* menuBase;
	UIImage* singlePlayer;
	UIElement* battleNet;
	UIElement* cinematics;
	UIElement* credits;
	UIElement* multiplayerOptions;
	UIElement* gateway;
	UIElement* exitDiablo;

};

#endif