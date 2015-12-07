#ifndef _Player_
#define _Player_
#include "Common.h"
//#include "Manager.h"

class Player{
public:
    SDL_Surface **TI;
    GLuint *gunTex;
    
    TTF_Font* font;
    
    GLfloat yrot; //Camera rotation variable
    GLfloat xpos, zpos; //Camera pos variable
    GLfloat walkbias, walkbiasangle; //Head-bobbing variables
    GLfloat lookupdown;
    bool fired;

    int score;
    int health;
    
    Player();
    
    void control(const Uint8* keyState);
    void draw();
    void drawHUD();
    void check();

    int getScore();
    int getHealth();
	bool getFired();
	GLfloat getX();
	GLfloat getZ();
	GLfloat getY();
    
    void setScore(int diff);
    void setHealth(int diff);
private:
};

#endif
