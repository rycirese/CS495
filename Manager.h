//JOE BONINO & RYAN CIRESE
//Manager Header File
#ifndef _Manager_
#define _Manager_
#include "Common.h"
#include "Player.h"
#include "World.h"
#include "Menu.h"
#include "Monster.h"
#include "Gun.h"

//Global
bool done;
SDL_Window *window;
Player *player;
const Uint8* keyState;
string inputText;
bool canShoot;
bool m;
bool gettingName;
int currentTime;
int hitTime;
int spawnTime;
int shootTime;
char buffer[1024];
int mHurtIndex;

//monster stuff
Monster *monsters[10];
int totalMonsters=0;
Mix_Chunk *mDeath;
Mix_Chunk *impact;
Mix_Chunk *mhurt;
string mHurt[6];
string impactPlayer[3];
string impactWall[2];
int impactWallIndex;
int impactPlayerIndex;

//Methods
void ALLSYSTEMSGO();
int main (int argc, char **argv);
void draw(const Uint8* keyState);
void spawnMonsters();
void createMonster(GLfloat x, GLfloat z, int type);
void monsterAI();
void shoot();
bool checkBulletCollision(GLfloat x,GLfloat z);
void outputScore();
void reset();
void animateGun();

#endif

