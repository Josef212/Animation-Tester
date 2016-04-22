#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Attributes.h"
#include "UIManager.h"
#include "UIElement.h"

enum phase
{
	BARBARIAN,
	BUTCHER,
	DIABLO
};

class Player :public Entity
{
public:
	Player();

	virtual ~Player();

	void draw();

	void drawDebug();

	bool entityUpdate(float internDT);

	bool entityPostUpdate();

	iPoint getMapPosition()const;
	iPoint getTilePosition() const;

	uint attack();

private:
	int mouseX, mouseY = 0;

	SDL_Texture* player_debug = NULL;

	phase currentPhase;
	std::map<std::pair<entityState, entityDirection>, Animation>*	barbarianAnimation = NULL;
	std::map<std::pair<entityState, entityDirection>, Animation>*	butcherAnimation = NULL;
	std::map<std::pair<entityState, entityDirection>, Animation>*	diabloAnimation = NULL;


public:
	Attributes* playerAttributes;

	UIElement* skillTree;
	UIElement* skillTreeActivated;
	UIElement* skillTreeClicked;

	int skillTreeState;
	float cooldown;
	bool habilityDone;
	iPoint habilityPosition;

};

#endif