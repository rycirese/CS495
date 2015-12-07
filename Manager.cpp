/////////////////////////////////////////////////
//          GROUP GAME - CS495-01-15F          //
//          JOE BONINO & RYAN CIRESE           //
//                                             //
//  DOOM like game where you progress through  //
//  waves of enemies. The more waves you beat  //
//  the harder the following waves become      //
//  increasingly harder. The main goal is to   //
//  get the highest score.                     //
/////////////////////////////////////////////////
#include "Manager.h"

int main(int argc, char **argv){
    ALLSYSTEMSGO(); //Sets Everything Up
    SDL_Event event;
    
    while(!done){
        currentTime = SDL_GetTicks();
        if(player->getHealth() <= 0){
            outputScore();
            reset();
            ALLSYSTEMSGO(); //Reset if Player DIES
        }
        const Uint8* keyState = SDL_GetKeyboardState(NULL); //Record Keystate
        if(!m) player->control(keyState); //Manage Player Controls
        if(keyState[SDL_SCANCODE_S]) m = false; //Turn Menu Mode Off (Start Game)
        if(keyState[SDL_SCANCODE_Q]){ done = true; reset(); SDL_Quit(); exit(0); break; }
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                reset();
                SDL_Quit();
                exit(0);
                break;
            }
        }
        draw(keyState); //Draws Everything
    }
    return 0;
}

void ALLSYSTEMSGO(){
    SDL_Quit();
    done = false;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    window = SDL_CreateWindow ("GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
    SDL_GL_CreateContext(window); //Associates the OpenGL commands to window 'window'.
    SDL_GL_SetSwapInterval(1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    TTF_Init();
    
    m = true; //Menu Mode is on (Loads Menu Not Game
    currentTime = 0;
    hitTime = 0;
    for(int i=0;i<10;i++) monsters[i] = NULL;
    
    genWorld();
    genMenu();
    player = new Player();
    
	createMonster(2,-5,1);
	createMonster(1,-5,2);
	createMonster(0,-5,3);
	createMonster(2,-5,4);
}

void createMonster(GLfloat x, GLfloat z, int type){
	//only allow 10 monsters
	if(totalMonsters<10){
		//find next NULL spot in monster array and create monster
		for(int i=0;i<10;i++)
			if(monsters[i]==NULL){
				monsters[i]=new Monster();
				if(type==1) monsters[i]=monsters[i]->getLightMonster();
				if(type==2) monsters[i]=monsters[i]->getMediumMonster();
				if(type==3) monsters[i]=monsters[i]->getHeavyMonster();
				if(type==4) monsters[i]=monsters[i]->getPatriarchMonster();

				monsters[i]->setX(x);
				monsters[i]->setZ(z);
				monsters[i]->setIndex(i);		//have monster keep track of his own index in monsters array
				break;
			}
	}
}

void monsterDeath(Monster*m){
	if(m!=NULL){
		monsters[m->getIndex()]=NULL;
		//Mix_PlayChannel (-1, mDeath, 0);
	}
}

void monsterAI(){
	for(int i = 0; i < 10; i++){
		if(monsters[i] != NULL){
			//Get Monster and Player Position Values
			GLfloat Mx=monsters[i]->getX();
			GLfloat Mz=monsters[i]->getZ();
			GLfloat Ms=monsters[i]->getSpeed();
			GLfloat Px=player->getX();
			GLfloat Pz=player->getZ();
            GLfloat Py=player->getY();
            
            //Move Monster Towards Player
			if(Mx<Px)		monsters[i]->setX(Mx+Ms);
			else if(Mx>Px)	monsters[i]->setX(Mx-Ms);
			if(Mz<Pz)		monsters[i]->setZ(Mz+Ms);
			else if(Mz>Pz)	monsters[i]->setZ(Mz-Ms);
            monsters[i]->setY(Py);
            
            //Player Take Damage Range and Invincable Time
            if(Mx < Px+0.2 && Mx > Px-0.2 && Mz < Pz+0.2 && Mz > Pz-0.2){
                if (currentTime > hitTime + 2000) {
                    player->setHealth(monsters[i]->getDamage());
                    hitTime = currentTime;
                }
            }
		}
	}
}

void shoot(){
 	const float DEG_TO_RAD = 0.0174532925f;
	int angle=player->yrot; //temp of players/cameras rotation variable
	GLfloat originX=player->xpos;
	GLfloat originZ=player->zpos;

	GLfloat adjz=0;
	GLfloat oppx=0;

	GLfloat acc=0.2; //accuracy. smaller is more accurate
	bool hit=false;

	angle=angle%360;
	if(angle<0) angle+=360; //define angle as always positive and 0 < angle < 360
	
	//	-X , -Z
	else if(angle>0&&angle<90){
		while(!hit){
			adjz -= acc; //z
			oppx = -(-adjz*(tan(angle*DEG_TO_RAD))); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	//	-X , +Z
	else if(angle>90&&angle<180){
		angle-=90;
		while(!hit){
			adjz+=acc; //z
			oppx = -((adjz)*(tan((90-angle)*DEG_TO_RAD))); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	//	+X , +Z
	else if(angle>180&&angle<270){
		angle-=180;
		while(!hit){
			adjz+=acc; //z
			oppx = (adjz)*(tan(angle*DEG_TO_RAD)); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	//	+X , -Z
	else if(angle>270&&angle<360){
		angle-=270;
		while(!hit){
			adjz-=acc; //z
			oppx = (-adjz)*(tan((90-angle)*DEG_TO_RAD)); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
}

bool checkBulletCollision(GLfloat x,GLfloat z){

	for(int i=0;i<10;i++){
		if(x>10||x<-10||z>10||z<-10) 
			return true; //bullet is out of bounds
		if(monsters[i]!=NULL){
			GLfloat xCol = abs(monsters[i]->getX()-x);
			GLfloat yCol = abs(monsters[i]->getZ()-z);
			//if bullet within 0.25 and 0.25
			if(xCol<0.2&&yCol<0.2){
   				monsters[i]->setHealth(monsters[i]->getHealth()-1);
				if(monsters[i]->getHealth()<1) monsterDeath(monsters[i]); //kill monster if health 0 or below

				return true; //hit!!
			}
		}
	}
	return false; //nothing hit
}

void draw(const Uint8* keyState){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Screen And The Depth Buffer
    glColor4f( 1.0f, 1.0f, 1.0f, 0.0f);
    glLoadIdentity();
    
	if(m) drawMenu(keyState);
    if(!m){
        monsterAI();
		player->draw();
        for(int i = 0; i < 10; i++){
            if(monsters[i] != NULL){
                monsters[i]->draw();
            }
        }
		if(player->getFired()){
			shoot();
			player->swapFired();
		}
		drawWorld(window);
	}
	SDL_GL_SwapWindow(window);
}

void outputScore(){
    string name = "GOD";
    int score =  player->getScore();
    
    char* x;
    sprintf(x, "%s scored: %d\n", name.c_str(), score);
    string out = x;
    
    ofstream f;
    f.open ("highscore.txt", std::ios_base::app);
    f << out;
    f.close();
}

void reset(){
    resetWorld();
}
