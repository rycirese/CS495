#ifndef _GUN_
#define _GUN_
#include <string>
using namespace std;

class Gun{
private:
	string name;
	string gunImage;
	string gunImage_fired;
	string shotgun[3];
	int damage; //how many hits to kill monster
	int fireRate; //when gun can fire again in seconds.
	int clipSize; //bullets in one clip
public:
	Gun();
	~Gun();
	Gun *getPistol();
	Gun *getShotgun();
	Gun *getRifle();

	string getName();
	string getGunImage();
	string getGunImage_fired();
	int getDamage();
	int getFireRate();
	int getClipSize();
	int getRange();
	string getSound();
};

#endif
