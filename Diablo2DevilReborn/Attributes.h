#ifndef _ATTRIBUTES_H_
#define _ATTRIBUTES_H_


class Attributes
{

private:

	//Max propiertes
	int level = 1;
	int hpMax = 0;
	int stamMax = 0;
	int furyMax = 0;
	int lvlMax = 0;
	int damage = 0;
	int currentExp = 0;
	int expRequired = 50;
	int dropExp = 10;

	//Variable propierties
	int hp;

	//Logic properties
	int range;

	//propierties * lvl
	int expLvl = 20;
	int hpLvl = 0;
	int stamLvl = 0;
	int furyLvl = 0;

public:

	//constructors
	Attributes(){}

	//assignment constructor
	Attributes(int baseHp, int basestam, int baseFury, int baseDamage, int _hpLvl, int _stamLvl, int _furyLvl, int _lvlMax, int range, int hp);

	//copy constructor
	Attributes(Attributes& atr);

	//destructor
	~Attributes();


	//methods
	void update();

	void gainExp(int exp);

	//tests-----------------------
	const int getExp() const;
	const int getHpMax() const;
	const int getStamMax() const;
	const int getFuryMax() const;
	const int getLevel() const;
	const int getDamage() const;
	const int getRange() const;
	const int getHp() const;
	const int getDropExp() const;

	void setExp(int value);
	void setHpMax(int value);
	void setStamMax(int value);
	void setFuryMax(int value);
	void setLevel(int value);
	void setDamage(int value);
	void setRange(int value);
	void setHp(int value);
	void setDropExp(int value);

};


#endif