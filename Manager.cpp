#include "Manager.h"

int main(int argc, char **argv){
    bool done = false;
    SDL_Event event;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    window = SDL_CreateWindow ("DOOM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GL_CreateContext(window); //Associates the OpenGL commands to window w.
    SDL_GL_SetSwapInterval(1);

    ALLSYSTEMSGO();

    while(!done){
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT: //handle quit requests
                    done = true;
                    break;
                default:
                    break;
            }
        }
    }
    return 0;
}

void ALLSYSTEMSGO(){
    genWorld(window);
}
