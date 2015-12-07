#ifndef _World_
#define _World_
#include "Common.h"

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
void resetWorld();

#endif
