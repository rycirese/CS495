#include "gun.h"

Gun::Gun(){
	//default to pistol
	name="pistol";
	image="//res//pistol.jpg";
	damage=1;
	fireRate=0.5;
	range=70;
}

Gun * Gun::getPistol(){
	name="pistol";
	image="//res//pistol.jpg";
	damage=1;
	fireRate=0.5;
	range=70;
	return this;
}
Gun * Gun::getShotgun(){
	name="pistol";
	image="//res//pistol.jpg";
	damage=3;
	fireRate=1.5;
	range=50;
	return this;
}
Gun * Gun::getSubMachineGun(){
	name="submachinegun";
	image="//res//submachinegun.jpg";
	damage=2;
	fireRate=0.2;
	range=70;
	return this;
}
Gun * Gun::getAssaultRifle(){
	name="submachinegun";
	image="//res//submachinegun.jpg";
	damage=3;
	fireRate=0.4;
	range=85;
	return this;
}
Gun * Gun::getSniper(){
	name="submachinegun";
	image="//res//submachinegun.jpg";
	damage=50;
	fireRate=2;
	range=100;
	return this;
}

string Gun::getName(){return name;}
string Gun::getImage(){return image;}
int Gun::getDamage(){return damage;}
double Gun::getFireRate(){return fireRate;}
int Gun::getClipSize(){return clipSize;}
int Gun::getRange(){return range;}