//JOE BONINO & RYAN CIRESE
//Monster Header File
#ifndef _MONSTER_
#define _MONSTER_
#include "Common.h"

class Monster{
private:
	string name;
	GLfloat speed;
	int health;
	int damage;
	GLfloat bounceBack;
    int points;

	int index;

	GLfloat xpos, zpos, ypos, yrot;
	GLfloat height;

	SDL_Surface**TI;
	GLuint * monsterTex;
public:
	Monster();
	~Monster();

	void draw();

	Monster *getLightMonster();
	Monster *getMediumMonster();
	Monster *getHeavyMonster();
	Monster *getPatriarchMonster();

	string getName();
	GLfloat getSpeed();
	int getHealth();
	int getDamage();
	int getIndex();
    int getPoints();
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ(); 
	Monster*getDead();
	
	void setY(GLfloat y);
	void setIndex(int i);
	void setX(GLfloat x);
	void setZ(GLfloat z);
	void setHealth(int h);
	void setSpeed(GLfloat s);
};

#endif

