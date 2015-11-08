#ifndef _Manager_
#define _Manager_
#include "Player.h"
#include "World.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>

//MAC INCLUDES
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#endif
//WINDOWS INCLUDES
#ifdef _WIN32
#include <SDL.h>
#include <SDL_opengl.h>
#endif

using namespace std;

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define PI 3.1415926535897932384626433832795

SDL_Window *window;

int main (int argc, char **argv);

void ALLSYSTEMSGO();
void draw();
void quit();

#endif
