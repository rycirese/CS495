#include "Monster.h"

Monster::Monster(){
    TI = new SDL_Surface*[2];
    monsterTex = new GLuint[2];
}

void Monster::draw(){
	//movement
	glPushMatrix();
    glTranslatef(xpos, 0, zpos); //Translate to Monster Position
    glRotatef(yrot, 0, 1, 0); //Rotate Monster
    glTranslatef(-xpos, (height/2)-.1, -zpos); //Translate back to Origin
    glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glBegin (GL_QUADS);
		glTexCoord2f(1,1);glVertex3f (xpos-0.25,-.1,zpos);
		glTexCoord2f(1,0);glVertex3f (xpos-0.25,height,zpos);
		glTexCoord2f(0,0);glVertex3f (xpos+0.25,height,zpos);
		glTexCoord2f(0,1);glVertex3f (xpos+0.25,-.1,zpos);
	glEnd ();
	glRotatef(20,0,1,0);
	glPopMatrix();
}

Monster * Monster::getLightMonster(){
	//set class variables
	name="light";
	speed=0.01;
	health=2;
	damage=1;
	height=0.5;
	bounceBack=0.5;
    points = 1;

	//texture belongs to monster
	TI[0] = IMG_Load ("data/textures/monsters/light.png");
	glGenTextures (1,&monsterTex[0]);
	glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glTexImage2D(GL_TEXTURE_2D,0,3,TI[0]->w,TI[0]->h,0,FORMAT_GUN,GL_UNSIGNED_BYTE,TI[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	SDL_FreeSurface(TI[0]);
	return this;
}
Monster * Monster::getMediumMonster(){
	name="medium";
	speed=0.015;
	health=3;
	damage=5;
	height=0.4;
	bounceBack=0.3;
    points = 5;
    
	//texture belongs to monster
	TI[0] = IMG_Load ("data/textures/monsters/med.png");
	glGenTextures (1,&monsterTex[0]);
	glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glTexImage2D(GL_TEXTURE_2D,0,3,TI[0]->w,TI[0]->h,0,FORMAT_GUN,GL_UNSIGNED_BYTE,TI[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	SDL_FreeSurface(TI[0]);

	return this;
}
Monster * Monster::getHeavyMonster(){
	name="heavy";
	speed=0.02;
	health=4;
	damage=10;
	height=0.5;
	bounceBack=0.2;
    points = 10;

	TI[0] = IMG_Load ("data/textures/monsters/heavy.png");
	glGenTextures (1,&monsterTex[0]);
	glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glTexImage2D(GL_TEXTURE_2D,0,3,TI[0]->w,TI[0]->h,0,FORMAT_GUN,GL_UNSIGNED_BYTE,TI[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	SDL_FreeSurface(TI[0]);
	return this;
}
Monster * Monster::getPatriarchMonster(){
	name="patriarch";
	speed=0.008;
	health=8;
	damage=25;
	height=0.5;
	bounceBack=0.05;
    points = 25;

	TI[0] = IMG_Load ("data/textures/monsters/patriarch.png");
	glGenTextures (1,&monsterTex[0]);
	glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glTexImage2D(GL_TEXTURE_2D,0,3,TI[0]->w,TI[0]->h,0,FORMAT_GUN,GL_UNSIGNED_BYTE,TI[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	SDL_FreeSurface(TI[0]);
	return this;
}
Monster * Monster::getDead(){
	name="dead";
	speed=0;
	health=0;
	damage=0;
	height=0;
	bounceBack=0;
    points = 0;
	TI[0] = NULL;
	monsterTex[0] = NULL;
	return this;
}

//getters
string Monster::getName(){return name;}
GLfloat Monster::getSpeed(){return speed;}
int Monster::getHealth(){return health;}
int Monster::getDamage(){return damage;}
int Monster::getPoints(){return points;}
GLfloat Monster::getX(){return xpos;}
GLfloat Monster::getY(){return yrot;}
GLfloat Monster::getZ(){return zpos;}
int Monster::getIndex(){return index;}
//setters
void Monster::setIndex(int i){index=i;}
void Monster::setX(GLfloat x){xpos=x;}
void Monster::setZ(GLfloat z){zpos=z;}
void Monster::setY(GLfloat y){yrot=y;}
void Monster::setHealth(int h){health=h;}
void Monster::setSpeed(GLfloat s){speed=s;}
