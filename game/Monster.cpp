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
}
Monster * Monster::getMediumMonster(){
	name="medium";
	speed=2;
	health=2;
	damage=2;
}
Monster * Monster::getHeavyMonster(){
	name="heavy";
	speed=4;
	health=4;
	damage=4;
}
Monster * Monster::getPatriarchMonster(){
	name="patriarch";
	speed=8;
	health=8;
	damage=8;
}

string Monster::getName(){return name;}
double Monster::getSpeed(){return speed;}
int Monster::getHealth(){return health;}
int Monster::getDamage(){return damage;}