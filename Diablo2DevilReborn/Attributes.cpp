#include "Attributes.h"

//assignment constructor
Attributes::Attributes(int baseHp, int basestam, int baseFury, int baseDamage, int _hpLvl, int _stamLvl, int _furyLvl, int _lvlMax, int range, int hp) : hpMax(baseHp), stamMax(basestam), furyMax(baseFury), damage(baseDamage), hpLvl(_hpLvl), stamLvl(_stamLvl), furyLvl(_furyLvl), lvlMax(_lvlMax), range(range), hp(hp)
{}

//copy constructor
Attributes::Attributes(Attributes& atr) : hpMax(atr.hpMax), stamMax(atr.stamMax), furyMax(atr.furyMax), lvlMax(atr.lvlMax), damage(atr.damage), currentExp(atr.currentExp), hpLvl(atr.hpLvl), stamLvl(atr.stamLvl), furyLvl(atr.furyLvl)
{}

//destructor
Attributes::~Attributes(){}


//methods
void Attributes::update()
{
	if (currentExp >= expRequired)
	{
		//need the change state the player 
		level++;
		hpMax += hpLvl;
		stamMax += stamLvl;
		furyMax += furyLvl;
		expRequired += expLvl;
		currentExp = 0;
	}

}

void Attributes::gainExp(int exp)
{
	currentExp += exp;
}

//tests-----------------------
//GETES
const int Attributes::getExp() const
{
	return currentExp;
}

const int Attributes::getHpMax() const
{
	return hpMax;
}

const int Attributes::getStamMax() const
{
	return stamMax;
}

const int Attributes::getFuryMax() const
{
	return furyMax;
}

const int Attributes::getLevel() const
{
	return level;
}

const int Attributes::getDamage() const
{
	return damage;
}

const int Attributes::getRange() const
{
	return range;
}
const int Attributes::getHp() const
{
	return hp;
}
const int Attributes::getDropExp() const
{
	return dropExp;
}

//SETES
void Attributes::setExp(int value)
{
	currentExp = value;
}

void Attributes::setHpMax(int value)
{
	hpMax = value;
}

void Attributes::setStamMax(int value)
{
	stamMax = value;
}

void Attributes::setFuryMax(int value)
{
	furyMax = value;
}

void Attributes::setLevel(int value)
{
	level = value;
}

void Attributes::setDamage(int value)
{
	damage = value;
}

void Attributes::setRange(int value)
{
	range = value;
}

void Attributes::setHp(int value)
{
	hp = value;
}

void Attributes::setDropExp(int value)
{
	dropExp = value;
}