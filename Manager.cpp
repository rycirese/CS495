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
#include <time.h>

//Set Up Everything Before Game Runs
void ALLSYSTEMSGO(){
    //Set Up Environment
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
    
    inputText = "";
    gettingName = true; //Get Name Mode (Loads Get Name Menu)
    m = true; //Menu Mode is on (Loads Menu Not Game)
    canShoot = true; //Flag Lock For Player Shooting
    currentTime = SDL_GetTicks(); //System Current Time, Used for Carious Timers
    hitTime = SDL_GetTicks(); //Time Used to Give Player Invinciblity Period
    spawnTime = SDL_GetTicks(); //Time Used for Spawning Monsters
    for(int i = 0; i < 10; i++) monsters[i] = NULL; //NULL All Monster Indexes
    
    //Create New World, Menu, and Player
    genWorld();
    genMenu();
    player = new Player();
    
    //Create 4 Initial Monsters
    createMonster( 5, -5, 1);
    createMonster(-5, -5, 2);
    createMonster( 5,  5, 3);
    createMonster(-5,  5, 4);
}

int main(int argc, char **argv){
    srand(time(NULL));
    ALLSYSTEMSGO(); //Sets Everything Up
    SDL_Event event;
    
    while(!done){
        currentTime = SDL_GetTicks();
        
        //Handle Player Death
        if(player->getHealth() <= 0){
            outputScore(); //Save Score to File
            reset(); //Reset World
            ALLSYSTEMSGO(); //Reset Game
        }
        
        //Handle Input
        const Uint8* keyState = SDL_GetKeyboardState(NULL); //Record Keystate;
        if(!m) player->control(keyState); //Manage Player Controls
        if(!gettingName && keyState[SDL_SCANCODE_S]) m = false; //Turn Menu Mode Off (Start Game)
        if(!gettingName && keyState[SDL_SCANCODE_Q]){ done = true; reset(); SDL_Quit(); exit(0); break; }
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_KEYDOWN){
                if(gettingName){
                    if(event.key.keysym.sym == SDLK_RETURN) gettingName = false;
                    if((event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)){
                        inputText.pop_back(); //Handle Backspace
                    }
                    if(gettingName && inputText.length() < 3){
                        if(event.key.keysym.sym >= 97 && event.key.keysym.sym <= 122){
                            inputText += (char)event.key.keysym.sym; //Append character
                        }
                    }
                }
                
                if(!gettingName && event.key.keysym.sym == SDLK_SPACE){
                    if(canShoot) shoot();
                    canShoot = false;
                }
            }
            if(event.type == SDL_KEYUP) {
                if(event.key.keysym.sym == SDLK_SPACE){ canShoot = true; }
            }
            if(event.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                reset();
                SDL_Quit();
                exit(0);
                break;
            }
        }

        //if(!gettingName) spawnMonsters();
        draw(keyState); //Draws Everything
    }
    return 0;
}

//Draw Routine
//Draws Player, World, Monsters, and Menu
void draw(const Uint8* keyState){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Screen And The Depth Buffer
    glColor4f( 1.0f, 1.0f, 1.0f, 0.0f);
    glLoadIdentity();
    
    if(m) drawMenu(gettingName, inputText, keyState);
    if(!gettingName && !m){
        monsterAI();
        player->draw();
        for(int i = 0; i < 10; i++){
            if(monsters[i] != NULL){
                monsters[i]->draw();
            }
        }
        drawWorld(window);
    }
    SDL_GL_SwapWindow(window);
}

//Spawns Another Monster at Random Location After 5 Seconds
void spawnMonsters(){
    currentTime = SDL_GetTicks();
    if (currentTime > spawnTime + 8000) {
        double sX = -9.7 + (rand() % 19); //Random Xpos between -9.7 and 9.3
        double sZ = -9.7 + (rand() % 19); //Random Zpos between -9.7 and 9.3
        int mT = 1 + (rand() % 4); //Random Monster Type between 1 and 4
        createMonster(sX, sZ, mT); //Create Random Monster
        spawnTime = SDL_GetTicks(); //Reset Spawn Time
    }
}

//Creates Monsters
void createMonster(GLfloat x, GLfloat z, int type){
	//Only Allow 10 monsters
	if(totalMonsters<10){
		//Find next NULL spot in monster array and create monster
        for(int i = 0; i < 10; i++){
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
}

//Moves Monster Towards Player Based on player position
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
            
            //Player Take Damage Range and Invinciblity Time
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

	//this method will fire a bullet from where ever player is in the players rotation.
	//it will create a right triangle with players position as 0,0 and angle equivalent to players rotation.
	//bullet will follow right angles hypotenuse

 	const float DEG_TO_RAD = 0.0174532925f;
	int angle=player->yrot; //temp of players/cameras rotation variable
	GLfloat originX=player->xpos;
	GLfloat originZ=player->zpos;
	GLfloat adjz=0; //adjacent side of right triangle/ refers to z on game
	GLfloat oppx=0; //opposite side of right triangle. refers to x on game

	GLfloat acc=0.2; //accuracy. smaller is more accurate
	bool hit=false;

	angle=angle%360;
	if(angle<0) angle+=360; //define angle as always positive and 0 < angle < 360

	//	-X , -Z
	if(angle>=0&&angle<90){
		while(!hit){
			adjz -= acc; //z
			oppx = -(-adjz*(tan(angle*DEG_TO_RAD))); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	//	-X , +Z
	else if(angle==90){
		while(!hit){
			oppx-=acc; //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	else if(angle>90&&angle<180){
		angle-=90;
		while(!hit){
			adjz+=acc; //z
			oppx = -((adjz)*(tan((90-angle)*DEG_TO_RAD))); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	//	+X , +Z
	else if(angle>=180&&angle<270){
		angle-=180;
		while(!hit){
			adjz+=acc; //z
			oppx = (adjz)*(tan(angle*DEG_TO_RAD)); //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
	//	+X , -Z
	else if(angle==270){
		while(!hit){
			oppx+=acc; //x
			hit = checkBulletCollision(oppx+originX,adjz+originZ);
		}
	}
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
	//x and z are cord to bullet
	//return true if hit, including wall, and false if nothing hit yet
	for(int i=0;i<10;i++){
        cout << "" << x << " and " << z << endl;
        if(x>10||x<-10||z>10||z<-10) {
            //cout << "OUT OF BOUNDS" << endl;
			return true; //bullet is out of bounds
        }
		if(monsters[i]!=NULL){
			GLfloat xCol = abs(monsters[i]->getX()-x);
			GLfloat zCol = abs(monsters[i]->getZ()-z);
			//if bullet within 0.2 and 0.2 away from monster
			if( xCol < 0.2 && zCol < 0.2){
   				monsters[i]->setHealth(monsters[i]->getHealth()-1);
				if(monsters[i]->getHealth()<1){
					monsters[i]=NULL; //kill monster if health 0 or below
					mDeath = Mix_LoadWAV("data/sounds/monster/death.wav");
					Mix_PlayChannel (-1,mDeath,0);
				}else{
					//mDeath = Mix_LoadWAV("data/sounds/monster/death.wav"); hurt noise
					//Mix_PlayChannel (-1,mDeath,0);
				}
				
				return true; //hit!!
			}
		}
	}
	return false; //nothing hit
}

//Sends Score to a .txt File
void outputScore(){
    //string name = "Batman";
    int score =  player->getScore();
    if(inputText.length() == 0) inputText = "___";
    
    string out = inputText + ": " + to_string(score) + "\n";
    
    ofstream f;
    f.open ("highscore.txt", std::ios_base::app);
    f << out;
    f.close();
}

void reset(){
    resetWorld();
}
