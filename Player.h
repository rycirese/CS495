//JOE BONINO & RYAN CIRESE
//Player Header File
#ifndef _Player_
#define _Player_
#include "Common.h"
#include "Gun.h"

class Player{
public:
    //Global
	Gun * gun;
    SDL_Surface **TI;
    GLuint *gunTex;
    TTF_Font* font;
    GLfloat yrot; //Camera rotation variable
    GLfloat xpos, zpos; //Camera pos variable
    GLfloat walkbias, walkbiasangle; //Head-bobbing variables
    GLfloat lookupdown;
    string s;
    string h;
    int score;
    int health;
	int gunTexIndex;
    
    //Methods
    Player();
    void control(const Uint8* keyState);
    void draw();
    void drawHUD();
    void check();
	void setGunTexIndex(int i);
    
    //Getters
    int getScore();
    int getHealth();
	GLfloat getX();
	GLfloat getZ();
	GLfloat getY();
    
    //Setters
    void addScore(int diff);
    void takeHealth(int diff);
private:
};

#endif
