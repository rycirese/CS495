//Player Class. Creates 'Player'
//  Holds all values pertaining to
//  the player
#include "Player.h"

Player::Player(){
    lookupdown = walkbias = walkbiasangle = 0.0f;
    GLfloat ratio = (GLfloat)1280/(GLfloat)720;
    GLfloat PI = 3.1415926535897932384626433832795;

	gun = new Gun();
	gun = gun->getPistol();
    
    font = TTF_OpenFont("data/fonts/Arial.ttf", 50);
    score = 0;
    health = 100;

	xpos = zpos = yrot = 0;
    
    glViewport(0, 0, (GLint)1280, (GLint)720); //Setup viewport
    glMatrixMode(GL_PROJECTION); //Change to the projection matrix and set our viewing volume.
    glLoadIdentity();
    glFrustum(-tan(45.0/360*PI)*0.1*ratio, tan(45.0/360*PI)*0.1*ratio, -tan(45.0/360*PI)*0.1, tan(45.0/360*PI)*0.1, 0.1, 100); //Set perspective
    glMatrixMode(GL_MODELVIEW); //Make sure we're chaning the model view and not the projection
    glLoadIdentity(); //Reset The View
    
    TI = new SDL_Surface *[2];
    gunTex = new GLuint[2];

	TI[0] = IMG_Load(gun->getGunImage().c_str());
	TI[1] = IMG_Load(gun->getGunImage_fired().c_str());
	for(int i=0;i<2;i++){
		//TEXTURE 1 (Gun Idle)
		glGenTextures(1, &gunTex[i]); //Create The Textur
		glBindTexture(GL_TEXTURE_2D, gunTex[i]); //Load in texture 1
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[i]->w, TI[i]->h, 0, FORMAT_GUN, GL_UNSIGNED_BYTE, TI[i]->pixels); //Generate The Texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glBindTexture( GL_TEXTURE_2D, NULL );
		SDL_FreeSurface(TI[i]);
	}
	gunTexIndex=0;//setup first texture to be idle
}
//Function to handle key press events
void Player::control(const Uint8* keyState){
    const float DEG_TO_RAD = 0.0174532925f;
    
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
    if(keyState[SDL_SCANCODE_RIGHT]){ //Right arrow key effectively turns the camera right by rotating the scene left
        yrot -= 3.0f;
    }
    if(keyState[SDL_SCANCODE_LEFT]){ //Left arrow key effectively turns the camera left by rotating the scene right
        yrot += 3.0f;
    }
	if(keyState[SDL_SCANCODE_A]){ //Q key makes the player strafe left
        xpos += (float)sin((yrot-90)*DEG_TO_RAD)*0.05f;
        zpos += (float)cos((yrot-90)*DEG_TO_RAD)*0.05f;
	}
	if(keyState[SDL_SCANCODE_D]){ //E key makes the player strafe right
        xpos += (float)sin((yrot+90)*DEG_TO_RAD)*0.05f;
        zpos += (float)cos((yrot+90)*DEG_TO_RAD)*0.05f;
	}
}

void Player::draw(){
    glColor4f( 1.0f, 1.0f, 1.0f, 0.0f);
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

//Draws HUD
//  Gun, Health, and Score are displayed
void Player::drawHUD(){
    string s = "SCORE: " + to_string(score);
    string h = "HEALTH: " + to_string(health);
    glEnable2D();
    glBindTexture(GL_TEXTURE_2D, gunTex[gunTexIndex]);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex3d(515, 0, 0);
        glTexCoord2d(1, 1); glVertex3d(765, 0, 0);
        glTexCoord2d(1, 0); glVertex3d(765, 250, 0);
        glTexCoord2d(0, 0); glVertex3d(515, 250, 0);
    glEnd();
    glDisable(GL_BLEND);
    
    glRenderText(font, 255, 0, 0, 0, 665, s);
    glRenderText(font, 255, 0, 0, 0, 0, h);
    
    glDisable2D();
}

//Keep Player Inside the Room
//Wall Check
void Player::check(){
    if(xpos >  9.74) xpos =  9.74;
    if(xpos < -9.74) xpos = -9.74;
    if(zpos >  9.74) zpos =  9.74;
    if(zpos < -9.74) zpos = -9.74;
}

//Getters
int Player::getScore(){ return score; }
int Player::getHealth(){ return health; }
GLfloat Player::getX(){return xpos;}
GLfloat Player::getZ(){return zpos;}
GLfloat Player::getY(){return yrot;}


void Player::setGunTexIndex(int i){gunTexIndex=i;}//so manager can change gun texture
void Player::addScore(int diff){ score = score+diff; }
void Player::takeHealth(int diff){ health = health-diff; }
