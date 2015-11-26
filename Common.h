#ifndef Common_h
#define Common_h

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
//MAC INCLUDES
#ifdef __APPLE__
#define FORMAT GL_BGRA
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#endif
//WINDOWS INCLUDES
#ifdef _WIN32
#define FORMAT GL_RGB
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#endif

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

using namespace std;

void glEnable2D();
void glDisable2D();

#endif
