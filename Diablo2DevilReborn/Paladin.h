#ifndef __PALADIN_H__
#define __PALADIN_H__

#include "Entity.h"
#include "Attributes.h"

class Paladin : public Entity
{
public: 
	Paladin();

	~Paladin();

	void draw();

	bool entityUpdate(float internDT);

	bool entityPostUpdate();

	void setInitVelocity();
	void updateVelocity(float internDT);
	void setAttackDirection();
	Attributes* getAttributes();

	float cooldown = 1.0f;
	float lastAttack = 0;

private:
	Attributes* paladinAttributes;
	std::map<std::pair<entityState, entityDirection>, Animation>* animation = NULL;
	void drawDebug();

	int visionRange;
	
};



#endif