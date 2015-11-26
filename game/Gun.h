#ifndef _GUN_
#define _GUN_
#include "Game.h"

class Gun{
private:
	string name;
	string image; 
	int damage; //how many hits to kill monster
	double fireRate; //when gun can fire again in seconds. If fireRate is 0.5 then gun can shoot every 0.5 second
	int clipSize; //bullets in one clip
	int range; //how far bullets go until they miss monster
public:
	Gun();
	~Gun();
	Gun *getPistol();
	Gun *getShotgun();
	Gun *getSubMachineGun();
	Gun *getAssaultRifle();
	Gun *getSniper();

	string getName();
	string getImage();
	int getDamage();
	double getFireRate();
	int getClipSize();
	int getRange();
};

#endif
