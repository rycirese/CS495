#ifndef _Manager_
#define _Manager_
#include "Common.h"
#include "Player.h"
#include "World.h"
#include "Menu.h"
#include "Monster.h"
#include "Gun.h"

SDL_Window *window;
Player *player;

//monster stuff
Monster *monsters[10];
int totalMonsters=0;
Mix_Chunk *mDeath;

int main (int argc, char **argv);

void createMonster(GLfloat x, GLfloat z, int type);
void monsterDeath(Monster*m);

void ALLSYSTEMSGO();
void draw();
void quit();


#endif

