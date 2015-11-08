/////////////////////////////////////////////////
//          GROUP GAME - CS495-01-15F          //
//          JOE BONINO & RYAN CIRESE           //
//                                             //
//  DOOM like game where you progress through  //
//  waves of enemies. The more waves you beat  //
//  the harder the following waves become      //
//  increasingly harder. The main goal is to   //
//  get the highest score.                     //
/////////////////////////////////////////////////
#include "Manager.h"

int main(int argc, char **argv){
    bool done = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    window = SDL_CreateWindow ("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GL_CreateContext(window); //Associates the OpenGL commands to window 'window'.
    SDL_GL_SetSwapInterval(1);

    ALLSYSTEMSGO(); //Sets Everything Up

    while(!done){
        while(SDL_PollEvent(&event)){
            const Uint8* keyState = SDL_GetKeyboardState(NULL);
            controlPlayer(keyState);
            if(event.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                quit();
                break;
            }
        }
        draw(); //Draws Everything
    }
    return 0;
}

void ALLSYSTEMSGO(){
    genPlayer();
    genWorld();
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Screen And The Depth Buffer
    glLoadIdentity();
    
    drawPlayer();
    drawWorld(window);
}

void quit(){
    quitWorld();
}
