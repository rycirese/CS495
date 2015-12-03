#include "Menu.h"

void menu(){
    glEnable2D();
    bool done = false;
    
    SDL_Event e;
    while(!done){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                quitWorld();
                break;
            }
            if(e.type == SDL_KEYDOWN) {
                if(e.key.keysym.sym == SDLK_s) return;
                if(e.key.keysym.sym == SDLK_h) return;
                if(e.key.keysym.sym == SDLK_i) return;
                if(e.key.keysym.sym == SDLK_q){
                    done = true;
                    quitWorld();
                    break;
                }
            }
        }
    }
    glDisable2D();
}