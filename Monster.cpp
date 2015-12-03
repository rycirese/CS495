#include "Monster.h"



Monster::Monster(){
	TI = new SDL_Surface*[2];
    monsterTex = new GLuint[2];
	
	
}

void Monster::draw(){
    
	//movement
	glPushMatrix();
    glRotatef(yrot,0,1,0);
	glTranslatef(xpos,(height/2)-.1,zpos);
	glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glBegin (GL_QUADS);
		glTexCoord2f(0,0);glVertex3f (xpos-0.25,-.1,zpos);
		glTexCoord2f(0,1);glVertex3f (xpos-0.25,height,zpos);
		glTexCoord2f(1,1);glVertex3f (xpos+0.25,height,zpos);
		glTexCoord2f(1,0);glVertex3f (xpos+0.25,-.1,zpos);
	glEnd ();
	glPopMatrix();
}

Monster * Monster::getLightMonster(){
	//set class variables
	name="light";
	speed=.005;
	health=1;
	damage=10;
	height=0.5;


	//texture belongs to monster
	TI[0] = IMG_Load ("data/textures/monsters/light.png");
	glGenTextures (3,&monsterTex[0]);
	glBindTexture (GL_TEXTURE_2D,monsterTex[0]);
	glTexImage2D(GL_TEXTURE_2D,0,3,TI[0]->w,TI[0]->h,0,FORMAT_GUN,GL_UNSIGNED_BYTE,TI[0]->pixels);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	SDL_FreeSurface(TI[0]);
	return this;
}
Monster * Monster::getMediumMonster(){
	name="medium";
	speed=2;
	health=2;
	damage=15;
	return this;
}
Monster * Monster::getHeavyMonster(){
	name="heavy";
	speed=4;
	health=4;
	damage=25;
	return this;
}
Monster * Monster::getPatriarchMonster(){
	name="patriarch";
	speed=8;
	health=8;
	damage=75;
	return this;
}

string Monster::getName(){return name;}
GLfloat Monster::getSpeed(){return speed;}
int Monster::getHealth(){return health;}
int Monster::getDamage(){return damage;}
GLfloat Monster::getX(){return xpos;}
GLfloat Monster::getY(){return yrot;}
GLfloat Monster::getZ(){return zpos;}
int Monster::getIndex(){return index;}


void Monster::setIndex(int i){index=i;}
void Monster::setX(GLfloat x){xpos=x;}
void Monster::setZ(GLfloat z){zpos=z;}
void Monster::setY(GLfloat y){yrot=y;}