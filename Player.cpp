//Player Class. Creates 'Player'
//  Holds all values pertaining to
//  the player
#include "Player.h"

///////////////////////////////////////////////////////////
//                   GLOBAL VARIABLES                    //
SDL_Surface **PTI;                                       //
GLuint gunTex[1]; //Storage for Gun Textures             //
int width = SCREEN_WIDTH;                                //
int height = SCREEN_HEIGHT;                              //
                                                         //
const float piover180 = 0.0174532925f;                   //
GLfloat yrot; //Camera rotation variable                 //
GLfloat xpos, zpos; //Camera pos variable                //
GLfloat walkbias, walkbiasangle; //Head-bobbing variables//
GLfloat lookupdown;                                      //
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
}

//Function to handle key press events
void controlPlayer(const Uint8* keyState){
    if(keyState[SDL_SCANCODE_RIGHT]){ //Right arrow key effectively turns the camera right by rotating the scene left
        yrot -= 6.0f;
    }
    if(keyState[SDL_SCANCODE_LEFT]){ //Left arrow key effectively turns the camera left by rotating the scene right
        yrot += 6.0f;
    }
    if(keyState[SDL_SCANCODE_UP]){ //Up arrow moves the player forward
        xpos -= (float)sin(yrot*piover180)*0.15f; //on the x-plane based on Player Direction
        zpos -= (float)cos(yrot*piover180)*0.15f; //on the z-plane based on Player Direction
        if (walkbiasangle >= 359.0f) walkbiasangle = 0.0f;
        else walkbiasangle += 45;
        walkbias = (float)sin(walkbiasangle*piover180)/45.0f; //Causes the player to bounce
    }
    if(keyState[SDL_SCANCODE_DOWN]){ //Down arrow key moves the player backwards
        xpos += (float)sin(yrot*piover180)*0.05f;
        zpos += (float)cos(yrot*piover180)*0.05f;
        if(walkbiasangle <= 1.0f) walkbiasangle = 359.0f;
        else walkbiasangle -= 45;
        walkbias = (float)sin(walkbiasangle*piover180)/60.0f;
    }
}

void drawPlayer(){
    GLfloat xtrans = -xpos; //Used For Player Translation On The X Axis
    GLfloat ztrans = -zpos; //Used For Player Translation On The Z Axis
    GLfloat ytrans = -walkbias-0.25f; //Used For Bouncing Motion Up And Down
    GLfloat sceneroty = 360.0f-yrot; //360 Degree Angle For Player Direction
    
    glRotatef(lookupdown, 1.0f, 0.0f , 0.0f); //Rotate Up And Down To Look Up And Down
    glRotatef(sceneroty, 0.0f, 1.0f , 0.0f); //Rotate Depending On Direction Player Is Facing
    glTranslatef(xtrans, ytrans, ztrans); //Translate The Scene Based On Player Position
}
