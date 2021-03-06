#include "gun.h"

Gun::Gun(){
	
}

Gun * Gun::getPistol(){
	name="pistol";
	gunImage="data/textures/guns/pistol/pistol.png";
	gunImage_fired="data/textures/guns/pistol/pistol_fired.png";
	damage=1;
	fireRate=500;
	return this;
}
Gun * Gun::getShotgun(){
	name="shotgun";
	gunImage="data/textures/guns/shotgun/shotgun.png";
	gunImage_fired="data/textures/guns/shotgun/shotgun_fired.png";
	damage=2;
	fireRate=1000;
	return this;
}
Gun * Gun::getRifle(){
	name="rifle";
	gunImage="data/textures/guns/rifle/rifle.png";
	gunImage_fired="data/textures/guns/rifle/rifle_fired.png";
	damage=3;
	fireRate=100;
	return this;
}

string Gun::getName(){return name;}
string Gun::getGunImage(){return gunImage;}
string Gun::getGunImage_fired(){return gunImage_fired;}
int Gun::getDamage(){return damage;}
int Gun::getFireRate(){return fireRate;}
int Gun::getClipSize(){return clipSize;}