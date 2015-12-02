#ifndef _Player_
#define _Player_
#include "Common.h"

class Player{
public:
    SDL_Surface **TI;
    GLuint *gunTex;
    
    GLfloat yrot; //Camera rotation variable
    GLfloat xpos, zpos; //Camera pos variable
    GLfloat walkbias, walkbiasangle; //Head-bobbing variables
    GLfloat lookupdown;
    bool fired;
    
    Player();
    
    void control(const Uint8* keyState);
    void draw();
    void drawHUD();
    void check();
private:
};

#endif
