//
//  Player.hpp
//  GROUP4
//
//  Created by Ryan Cirese on 11/7/15.
//  Copyright © 2015 Ryan Cirese. All rights reserved.
//

#ifndef _Player_
#define _Player_

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 720
#define PI 3.1415926535897932384626433832795

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
#define FORMAT GL_RGBA
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#endif

using namespace std;

void genPlayer();
void controlPlayer(const Uint8* keyState);
void drawPlayer();

#endif
