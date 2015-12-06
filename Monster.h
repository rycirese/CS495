#ifndef _MONSTER_
#define _MONSTER_
#include "Common.h"

class Monster{
private:
	string name;
	GLfloat speed;
	int health;
	int damage;

	int index;

	GLfloat xpos, zpos, yrot;
	GLfloat height;

	SDL_Surface**TI;
	GLuint * monsterTex;
public:
	Monster();
	~Monster();

	void draw(GLfloat playerX, GLfloat playerZ, GLfloat playerY);

	Monster *getLightMonster();
	Monster *getMediumMonster();
	Monster *getHeavyMonster();
	Monster *getPatriarchMonster();

	string getName();
	GLfloat getSpeed();
	int getHealth();
	int getDamage();
	int getIndex();
	GLfloat getX();
	GLfloat getY();
	GLfloat getZ(); 
	
	void setY(GLfloat y);
	void setIndex(int i);
	void setX(GLfloat x);
	void setZ(GLfloat z);
};

#endif

