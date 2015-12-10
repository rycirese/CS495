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
//shooting stuff
const float DEG_TO_RAD = 0.0174532925f;
GLfloat acc=0.15; //accuracy. smaller is more accurate
int angle; //temp of players/cameras rotation variable
GLfloat adjz=0; //adjacent side of right triangle/ refers to z on game
GLfloat oppx=0; //opposite side of right triangle. refers to x on game
bool hit=false;
GLfloat xCol;
GLfloat zCol;

//monster stuff
Monster *monsters[10];
int totalMonsters=0;
Mix_Chunk *gameSound[16];

int impactWallIndex;
int impactPlayerIndex;
int gunSound;

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
