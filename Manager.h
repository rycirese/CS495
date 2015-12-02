#ifndef _Manager_
#define _Manager_
#include "Common.h"
#include "Player.h"
#include "World.h"

SDL_Window *window;
Player *player;

int main (int argc, char **argv);

void ALLSYSTEMSGO();
void draw();
void quit();

#endif
