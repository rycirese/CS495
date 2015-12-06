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
bool m;

//monster stuff
Monster *monsters[10];
int totalMonsters=0;
Mix_Chunk *mDeath;

int main (int argc, char **argv);

void createMonster(GLfloat x, GLfloat z, int type);
void monsterDeath(Monster*m);
void monsterAI();
void ALLSYSTEMSGO();
void draw(const Uint8* keyState);
void quit();



#endif

