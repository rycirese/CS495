//Player Class. Creates 'Player'
//  Holds all values pertaining to
//  the player
#include "Player.h"

///////////////////////////////////////////////////////////
//                   GLOBAL VARIABLES                    //
SDL_Surface **PTI;                                       //
GLuint gunTex[2]; //Storage for Gun Textures             //
int width = SCREEN_WIDTH;                                //
int height = SCREEN_HEIGHT;                              //
                                                         //
const float DEG_TO_RAD = 0.0174532925f;                  //
GLfloat yrot; //Camera rotation variable                 //
GLfloat xpos, zpos; //Camera pos variable                //
GLfloat walkbias, walkbiasangle; //Head-bobbing variables//
GLfloat lookupdown;                                      //
bool fired;                                              //
///////////////////////////////////////////////////////////


void genPlayer(){
    lookupdown = walkbias = walkbiasangle = 0.0f;
    
    GLfloat ratio; //Height / width ration
    if(height == 0) height = 1; //Protect against a divide by zero
    ratio = (GLfloat)width/(GLfloat)height;
    
    glViewport(0, 0, (GLint)width, (GLint)height); //Setup viewport
    glMatrixMode(GL_PROJECTION); //Change to the projection matrix and set our viewing volume.
    glLoadIdentity();
    glFrustum(-tan(45.0/360*PI)*0.1*ratio, tan(45.0/360*PI)*0.1*ratio, -tan(45.0/360*PI)*0.1, tan(45.0/360*PI)*0.1, 0.1, 100); //Set perspective
    glMatrixMode(GL_MODELVIEW); //Make sure we're chaning the model view and not the projection
    glLoadIdentity(); //Reset The View
    
    PTI = new SDL_Surface *[2];
    
    PTI[0] = IMG_Load("data/textures/guns/pistol.png"); //Gun Texture
    
    //TEXTURE 1 (Gun Idle)
    glGenTextures(3, &gunTex[0]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, gunTex[0]); //Load in texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, PTI[0]->w, PTI[0]->h, 0, FORMAT, GL_UNSIGNED_BYTE, PTI[0]->pixels); //Generate The Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    SDL_FreeSurface(PTI[0]);
}

//Function to handle key press events
void controlPlayer(const Uint8* keyState){
    fired = false;
    if(keyState[SDL_SCANCODE_RIGHT]){ //Right arrow key effectively turns the camera right by rotating the scene left
        yrot -= 3.0f;
    }
    if(keyState[SDL_SCANCODE_LEFT]){ //Left arrow key effectively turns the camera left by rotating the scene right
        yrot += 3.0f;
    }
    if(keyState[SDL_SCANCODE_UP]){ //Up arrow moves the player forward
        xpos -= (float)sin(yrot*DEG_TO_RAD)*0.05f; //on the x-plane based on Player Direction
        zpos -= (float)cos(yrot*DEG_TO_RAD)*0.05f; //on the z-plane based on Player Direction
        if (walkbiasangle >= 359.0f) walkbiasangle = 0.0f;
        else walkbiasangle += 10;
        walkbias = (float)sin(walkbiasangle*DEG_TO_RAD)/20.0f; //Causes the player to bounce
    }
    if(keyState[SDL_SCANCODE_DOWN]){ //Down arrow key moves the player backwards
        xpos += (float)sin(yrot*DEG_TO_RAD)*0.05f;
        zpos += (float)cos(yrot*DEG_TO_RAD)*0.05f;
        if(walkbiasangle <= 1.0f) walkbiasangle = 359.0f;
        else walkbiasangle -= 10;
        walkbias = (float)sin(walkbiasangle*DEG_TO_RAD)/20.0f;
    }
    if(keyState[SDL_SCANCODE_SPACE]) fired = true;
}

void drawPlayer(){
    GLfloat xtrans = -xpos; //Used For Player Translation On The X Axis
    GLfloat ztrans = -zpos; //Used For Player Translation On The Z Axis
    GLfloat ytrans = -walkbias-0.25f; //Used For Bouncing Motion Up And Down
    GLfloat sceneroty = 360.0f-yrot; //360 Degree Angle For Player Direction
    
    glRotatef(lookupdown, 1.0f, 0.0f , 0.0f); //Rotate Up And Down To Look Up And Down
    glRotatef(sceneroty, 0.0f, 1.0f , 0.0f); //Rotate Depending On Direction Player Is Facing
    glTranslatef(xtrans, ytrans, ztrans); //Translate The Scene Based On Player Position
    
    drawGun();
}

void drawGun(){
    glEnable2D();
    
    glBindTexture(GL_TEXTURE_2D, gunTex[0]);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex2d(515, 0);
        glTexCoord2d(1, 1); glVertex2d(765, 0);
        glTexCoord2d(1, 0); glVertex2d(765, 250);
        glTexCoord2d(0, 0); glVertex2d(515, 250);
    glEnd();
    glDisable(GL_BLEND);
    
    glDisable2D();
}

void glEnable2D(){
    int vPort[4];
    
    glGetIntegerv(GL_VIEWPORT, vPort);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void glDisable2D(){
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
