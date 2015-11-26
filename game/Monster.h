#ifndef _MONSTER_
#define _MONSTER_
#include "Game.h"

class Monster{
private:
	string name;
	double speed;
	int health;
	int damage;
public:
	Monster();
	~Monster();

	Monster *getLightMonster();
	Monster *getMediumMonster();
	Monster *getHeavyMonster();
	Monster *getPatriarchMonster();

	string getName();
	double getSpeed();
	int getHealth();
	int getDamage();
};

#endif
