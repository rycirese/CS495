#include "Monster.h"

Monster::Monster(){
	//default new monster as light
	name="light";
	speed=1;
	health=1;
	damage=1;
}

Monster * Monster::getLightMonster(){
	name="light";
	speed=1;
	health=1;
	damage=1;
	return this;
}
Monster * Monster::getMediumMonster(){
	name="medium";
	speed=2;
	health=2;
	damage=2;
	return this;
}
Monster * Monster::getHeavyMonster(){
	name="heavy";
	speed=4;
	health=4;
	damage=4;
	return this;
}
Monster * Monster::getPatriarchMonster(){
	name="patriarch";
	speed=8;
	health=8;
	damage=8;
	return this;
}

string Monster::getName(){return name;}
double Monster::getSpeed(){return speed;}
int Monster::getHealth(){return health;}
int Monster::getDamage(){return damage;}