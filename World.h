#ifndef _World_
#define _World_

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
#define FORMAT GL_RGB
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#endif

using namespace std;

//Build Vertex Structure
typedef struct{
    float x, y, z; //3D Coordinates
    float u, v; //Texture Coordinates
    int t; //Texture Code
} vertex;

//Build Triangle Structure
typedef struct{
    vertex vertex[3]; //Array Of Three Vertices
} triangle;

//Build Sector Structure
typedef struct{
    int numTriangles; //Number Of Triangles In Sector
    triangle *triangle; //Pointer To Array Of Triangles
} sector;

void genWorld();

void initWorld();
void LoadGLTextures();
void readstr(FILE *f, char *string);
void setupWorld(string worldFile);
void drawWorld(SDL_Window *window);
void quitWorld();

#endif
