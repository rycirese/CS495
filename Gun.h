#ifndef _GUN_
#define _GUN_
#include <string>
using namespace std;

class Gun{
private:
	string name;
	string gunImage;
	string gunImage_fired;
	string gun_sound;
	int damage; //how many hits to kill monster
	int fireRate; //when gun can fire again in seconds. If fireRate is 0.5 then gun can shoot every 0.5 second
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
};

#endif
