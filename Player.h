//JOE BONINO & RYAN CIRESE
//Player Header File
#ifndef _Player_
#define _Player_
#include "Common.h"

class Player{
public:
    //Global
    SDL_Surface **TI;
    GLuint *gunTex;
    TTF_Font* font;
    GLfloat yrot; //Camera rotation variable
    GLfloat xpos, zpos; //Camera pos variable
    GLfloat walkbias, walkbiasangle; //Head-bobbing variables
    GLfloat lookupdown;
    int score;
    int health;
    
    //Methods
    Player();
    void control(const Uint8* keyState);
    void draw();
    void drawHUD();
    void check();
    
    //Getters
    int getScore();
    int getHealth();
	GLfloat getX();
	GLfloat getZ();
	GLfloat getY();
    
    //Setters
    void setScore(int diff);
    void setHealth(int diff);
private:
};

#endif
