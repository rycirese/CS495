#ifndef _Manager_
#define _Manager_
#include "Common.h"
#include "Player.h"
#include "World.h"
#include "Menu.h"

SDL_Window *window;
Player *player;
bool m;

int main (int argc, char **argv);

void ALLSYSTEMSGO();
void draw(const Uint8* keyState);
void quit();

#endif
