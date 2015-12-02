//Player Class. Creates 'Player'
//  Holds all values pertaining to
//  the player
#include "Player.h"

Player::Player(){
    lookupdown = walkbias = walkbiasangle = 0.0f;
    GLfloat ratio = (GLfloat)1280/(GLfloat)720;
    GLfloat PI = 3.1415926535897932384626433832795;
    
    xpos = zpos = yrot = 0;
    
    glViewport(0, 0, (GLint)1280, (GLint)720); //Setup viewport
    glMatrixMode(GL_PROJECTION); //Change to the projection matrix and set our viewing volume.
    glLoadIdentity();
    glFrustum(-tan(45.0/360*PI)*0.1*ratio, tan(45.0/360*PI)*0.1*ratio, -tan(45.0/360*PI)*0.1, tan(45.0/360*PI)*0.1, 0.1, 100); //Set perspective
    glMatrixMode(GL_MODELVIEW); //Make sure we're chaning the model view and not the projection
    glLoadIdentity(); //Reset The View
    
    TI = new SDL_Surface *[2];
    gunTex = new GLuint[2];
    
    TI[0] = IMG_Load("data/textures/guns/pistol.png"); //Gun Texture
    
    //TEXTURE 1 (Gun Idle)
    glGenTextures(3, &gunTex[0]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, gunTex[0]); //Load in texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[0]->w, TI[0]->h, 0, FORMAT_GUN, GL_UNSIGNED_BYTE, TI[0]->pixels); //Generate The Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    SDL_FreeSurface(TI[0]);
}

//Function to handle key press events
void Player::control(const Uint8* keyState){
    const float DEG_TO_RAD = 0.0174532925f;
    
    if(keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D]){ //Right arrow key effectively turns the camera right by rotating the scene left
        yrot -= 3.0f;
    }
    if(keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A]){ //Left arrow key effectively turns the camera left by rotating the scene right
        yrot += 3.0f;
    }
    if(keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_W]){ //Up arrow moves the player forward
        xpos -= (float)sin(yrot*DEG_TO_RAD)*0.05f; //on the x-plane based on Player Direction
        zpos -= (float)cos(yrot*DEG_TO_RAD)*0.05f; //on the z-plane based on Player Direction
        if (walkbiasangle >= 359.0f) walkbiasangle = 0.0f;
        else walkbiasangle += 10;
        walkbias = (float)sin(walkbiasangle*DEG_TO_RAD)/20.0f; //Causes the player to bounce
    }
    if(keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_S]){ //Down arrow key moves the player backwards
        xpos += (float)sin(yrot*DEG_TO_RAD)*0.05f;
        zpos += (float)cos(yrot*DEG_TO_RAD)*0.05f;
        if(walkbiasangle <= 1.0f) walkbiasangle = 359.0f;
        else walkbiasangle -= 10;
        walkbias = (float)sin(walkbiasangle*DEG_TO_RAD)/20.0f;
    }
    if(keyState[SDL_SCANCODE_SPACE]) fired = true;
	if(keyState[SDL_SCANCODE_Q]){ //Q key makes the player strafe left
        xpos += (float)sin((yrot-90)*DEG_TO_RAD)*0.05f;
        zpos += (float)cos((yrot-90)*DEG_TO_RAD)*0.05f;
	}
	if(keyState[SDL_SCANCODE_E]){ //E key makes the player strafe right
        xpos += (float)sin((yrot+90)*DEG_TO_RAD)*0.05f;
        zpos += (float)cos((yrot+90)*DEG_TO_RAD)*0.05f;
	}
}

void Player::draw(){
    GLfloat xtrans = -xpos; //Used For Player Translation On The X Axis
    GLfloat ztrans = -zpos; //Used For Player Translation On The Z Axis
    GLfloat ytrans = -walkbias-0.25f; //Used For Bouncing Motion Up And Down
    GLfloat sceneroty = 360.0f-yrot; //360 Degree Angle For Player Direction
    
    glRotatef(0.0f, 1.0f, 0.0f , 0.0f); //Rotate Up And Down To Look Up And Down
    glRotatef(sceneroty, 0.0f, 1.0f , 0.0f); //Rotate Depending On Direction Player Is Facing
    glTranslatef(xtrans, ytrans, ztrans); //Translate The Scene Based On Player Position
    
    check();
    drawHUD();
}

void Player::drawHUD(){
    glEnable2D();
    
    glBindTexture(GL_TEXTURE_2D, gunTex[0]);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex3d(515, 0, 0);
        glTexCoord2d(1, 1); glVertex3d(765, 0, 0);
        glTexCoord2d(1, 0); glVertex3d(765, 250, 0);
        glTexCoord2d(0, 0); glVertex3d(515, 250, 0);
    glEnd();
    glDisable(GL_BLEND);
    
    glDisable2D();
}

void Player::check(){
    if(xpos >  9.74) xpos =  9.74;
    if(xpos < -9.74) xpos = -9.74;
    if(zpos >  9.74) zpos =  9.74;
    if(zpos < -9.74) zpos = -9.74;
}
