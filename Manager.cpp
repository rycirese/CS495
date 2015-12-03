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
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    TTF_Init();
    
    ALLSYSTEMSGO(); //Sets Everything Up

    m = true; //Menu Mode is on (Loads Menu Not Game
    while(!done){
        const Uint8* keyState = SDL_GetKeyboardState(NULL); //Record Keystate
        if(!m) player->control(keyState); //Manage Player Controls
        if(keyState[SDL_SCANCODE_S]) m = false; //Turn Menu Mode Off (Start Game)
        if(keyState[SDL_SCANCODE_Q]){ done = true; quit(); break; }
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                quit();
                break;
            }
        }
        draw(keyState); //Draws Everything
    }
    return 0;
}

void ALLSYSTEMSGO(){
    genWorld();
    genMenu();
    player = new Player();
}

void draw(const Uint8* keyState){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Screen And The Depth Buffer
    glColor4f( 1.0f, 1.0f, 1.0f, 0.0f);
    glLoadIdentity();
    
    if(m) drawMenu(keyState);
    if(!m){
        player->draw();
        drawWorld(window);
    }
    SDL_GL_SwapWindow(window); //Draw it to the screen
}

void quit(){
    quitWorld();
}
