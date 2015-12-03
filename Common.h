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
#define FORMAT_GUN GL_BGRA
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif
//WINDOWS INCLUDES
#ifdef _WIN32
#define FORMAT GL_RGB
#define FORMAT_GUN GL_RGBA
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720

using namespace std;

void glRenderText(TTF_Font *font, int r, int g, int b, int x, int y, string text);
void glEnable2D();
void glDisable2D();

#endif
