#ifndef _World_
#define _World_

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 400
#define PI 3.1415926535897932384626433832795

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

//Build Vertex Structure
typedef struct{
    float x, y, z; //3D Coordinates
    float u, v; //Texture Coordinates
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

int initGL();
int LoadGLTextures();
void readstr(FILE *f, char *string);
void SetupWorld(string worldFile);
void draw(SDL_Window *window);
void Quit(int returnCode);
int resizeWindow(int width, int height);
void genWorld(SDL_Window *window);

#endif