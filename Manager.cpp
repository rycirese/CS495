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
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    TTF_Init();
    
    inputText = "";
    gettingName = true; //Get Name Mode (Loads Get Name Menu)
    m = true; //Menu Mode is on (Loads Menu Not Game)
    canShoot = true; //Flag Lock For Player Shooting
    currentTime = SDL_GetTicks(); //System Current Time, Used for Carious Timers
    hitTime = SDL_GetTicks(); //Time Used to Give Player Invinciblity Period
    spawnTime = SDL_GetTicks(); //Time Used for Spawning Monsters
    //for(int i = 0; i < 10; i++) monsters[i] = NULL; //NULL All Monster Indexes
    
    //Create New World, Menu, and Player
    genWorld();
    genMenu();
    player = new Player();
	for(int i=0;i<10;i++){
		monsters[i] = new Monster();
		monsters[i] = monsters[i]->getDead();
	}
	mHurtIndex=0;
	impactWallIndex=0;
	impactPlayerIndex=0;

	//0-4 are monster noises
	//5 patriarch spawn
	//6-7 metal impact
	//8-10 player impact
	//11 pistol shoot
	//12 shotgun shoot
	//13 rifle shoot
	//14 zombie death

	gameSound[0] = Mix_LoadWAV("data/sounds/monster/growl.wav");
	gameSound[1] = Mix_LoadWAV("data/sounds/monster/kill_you.wav");
	gameSound[2] = Mix_LoadWAV("data/sounds/monster/moan.wav");
	gameSound[3] = Mix_LoadWAV("data/sounds/monster/mummy.wav");
	gameSound[4] = Mix_LoadWAV("data/sounds/monster/zombie_attacked.wav");
	gameSound[5] = Mix_LoadWAV("data/sounds/monster/patriarch_spawn.wav");

	gameSound[6] = Mix_LoadWAV("data/sounds/impact/metal_bang.wav");
	gameSound[7] = Mix_LoadWAV("data/sounds/impact/metal_bang2.wav");

	gameSound[8] = Mix_LoadWAV("data/sounds/impact/bite.wav");
	gameSound[9] = Mix_LoadWAV("data/sounds/impact/flesh.wav");
	gameSound[10] = Mix_LoadWAV("data/sounds/impact/flesh_bone.wav");

	gameSound[11] = Mix_LoadWAV("data/sounds/weapons/pistol/shoot.wav");
	gameSound[12] = Mix_LoadWAV("data/sounds/weapons/shotgun/shoot.wav");
	gameSound[13] = Mix_LoadWAV("data/sounds/weapons/rifle/shoot.wav");

	gameSound[14] = Mix_LoadWAV("data/sounds/monster/death.wav");
	
    
    //Create 4 Initial Monsters
    //createMonster( 0, -5, 4);
}

int main(int argc, char **argv){
    srand(time(NULL));
    ALLSYSTEMSGO(); //Sets Everything Up
    SDL_Event event;
    bool rifle=false,shotgun=false;
    while(!done){
        currentTime = SDL_GetTicks();
        //Handle Player Death
        if(player->getHealth() <= 0){
            outputScore(); //Save Score to File
            reset(); //Reset World
            ALLSYSTEMSGO(); //Reset Game
        }
        
        //Handle Input
        keyState = SDL_GetKeyboardState(NULL); //Record Keystate;
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
					if( shootTime+player->gun->getFireRate() < SDL_GetTicks() ) canShoot=true;
					if(canShoot){ 
						canShoot=false;
						shootTime = SDL_GetTicks();
						shoot();
						player->setGunTexIndex(1); //set texture to firing
					}
                }
				if(!gettingName && event.key.keysym.sym == SDLK_p){
                    for(int i=0;i<10;i++)
						if(monsters[i]->getName()!="dead"){
							//pause
							monsters[i]->setSpeed(0);
						}
					
                }
            }
            if(event.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                reset();
                SDL_Quit();
                exit(0);
                break;
            }
        }

        if(!m){
			if(!shotgun)
				if(currentTime>60000){
					player->gun = player->gun->getShotgun();
					if(!shotgun)player->loadText();
					shotgun=true;
				}
			if(!rifle)
				if(currentTime>120000){
					player->gun = player->gun->getRifle();
					if(!rifle)player->loadText();
					rifle=true;
				}
			spawnMonsters();
		}
		animateGun();
        draw(keyState); //Draws Everything
    }
    return 0;
}

//animates shooting and shotgun
void animateGun(){
	if( shootTime+150 < SDL_GetTicks() ) player->setGunTexIndex(0);
	if(player->gun->getName()=="shotgun"){
		if( shootTime+225<SDL_GetTicks() ) player->setGunTexIndex(2);
		if( shootTime+350<SDL_GetTicks() ) player->setGunTexIndex(3);
		if( shootTime+475<SDL_GetTicks() ) player->setGunTexIndex(4);
		if( shootTime+600<SDL_GetTicks() ) player->setGunTexIndex(3);
		if( shootTime+725<SDL_GetTicks() ) player->setGunTexIndex(2);
		if( shootTime+850<SDL_GetTicks() ) player->setGunTexIndex(0);
	}
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
            if(monsters[i]->getName() != "dead"){
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
    if (currentTime > spawnTime + 2000) {
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
			if(monsters[i]->getName() == "dead"){
				if(type==1) monsters[i]=monsters[i]->getLightMonster();
				if(type==2) monsters[i]=monsters[i]->getMediumMonster();
				if(type==3) monsters[i]=monsters[i]->getHeavyMonster();
				if(type==4){ 
					monsters[i]=monsters[i]->getPatriarchMonster();
					Mix_PlayChannel (-1,gameSound[5],0);
				}

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
		if(monsters[i]->getName() != "dead"){
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
                    player->takeHealth(monsters[i]->getDamage());
                    hitTime = currentTime;
					Mix_PlayChannel (-1,gameSound[rand()%2+8],0);
                }
            }
		}
	}
}

//shoots bullet
void shoot(){
	//play gun shot noise
	if(player->gun->getName()=="pistol") Mix_PlayChannel (-1,gameSound[11],0);
	else if(player->gun->getName()=="shotgun") Mix_PlayChannel (-1,gameSound[12],0);
	else Mix_PlayChannel (-1,gameSound[13],0);

	//this method will fire a bullet from where ever player is in the players rotation.
	//it will create a right triangle with players position as 0,0 and angle equivalent to players rotation.
	//bullet will follow right angles hypotenuse

	angle=player->yrot; //temp of players/cameras rotation variable
	adjz=0; //adjacent side of right triangle/ refers to z on game
	oppx=0; //opposite side of right triangle. refers to x on game
	hit=false;

	//if yrot or player angle is over 360, put it in bounds of 360-0
	angle=angle%360;
	if(angle<0) angle+=360; //define angle as always positive and 0 < angle < 360

	//	-X , -Z
 	if(angle>=0&&angle<90){
		while(!hit){
			//traject bullet with a velocity traveling -z and -x from players position
			adjz -= acc; //z
			oppx = -(-adjz*(tan(angle*DEG_TO_RAD))); //x
			hit = checkBulletCollision(oppx+player->xpos,adjz+player->zpos);
		}
	}
	//	-X , +Z
	else if(angle==90){
		while(!hit){
			//traject bullet with a velocity traveling -x from players position
			oppx-=acc; //x
			hit = checkBulletCollision(oppx+player->xpos,adjz+player->zpos);
		}
	}
	else if(angle>90&&angle<180){
		angle-=90; //so right triangle can be made
		while(!hit){
			//traject bullet with a velocity traveling +z and -x from players position
			adjz+=acc; //z
			oppx = -((adjz)*(tan((90-angle)*DEG_TO_RAD))); //x
			hit = checkBulletCollision(oppx+player->xpos,adjz+player->zpos);
		}
	}
	//	+X , +Z
	else if(angle>=180&&angle<270){
		angle-=180; //for right triangle
		while(!hit){
			//traject bullet with a velocity traveling +z and +x from players position
			adjz+=acc; //z
			oppx = (adjz)*(tan(angle*DEG_TO_RAD)); //x
			hit = checkBulletCollision(oppx+player->xpos,adjz+player->zpos);
		}
	}
	//	+X , -Z
	else if(angle==270){
		while(!hit){
			//traject bullet with a velocity traveling +x from players position
			oppx+=acc; //x
			hit = checkBulletCollision(oppx+player->xpos,adjz+player->zpos);
		}
	}
	else if(angle>270&&angle<360){
		angle-=270; //for right triangle
		while(!hit){
			//traject bullet with a velocity traveling -z and +x from players position
			adjz-=acc; //z
			oppx = (-adjz)*(tan((90-angle)*DEG_TO_RAD)); //x
			hit = checkBulletCollision(oppx+player->xpos,adjz+player->zpos);
		}
	}
}
//checks if bullet is close to every monster
bool checkBulletCollision(GLfloat x,GLfloat z){
	//x and z are cord to bullet
	//return true if hit, including wall, and false if nothing hit yet
	for(int i=0;i<10;i++){
        if(x>10||x<-10||z>10||z<-10) {
			Mix_PlayChannel (-1,gameSound[rand() % 1 + 6],0);
			return true; //bullet is out of bounds
        }
		if(monsters[i]->getName()!="dead"){
			xCol = abs(monsters[i]->getX()-x);
			zCol = abs(monsters[i]->getZ()-z);
			//if bullet within 0.2 and 0.2 away from monster
			if( xCol < 0.2 && zCol < 0.2){
   				monsters[i]->setHealth(monsters[i]->getHealth()-player->gun->getDamage());
				if(monsters[i]->getHealth()<1){
                    player->addScore(monsters[i]->getPoints());
                    monsters[i] = monsters[i]->getDead(); //kill monster if health 0 or below
					Mix_PlayChannel (-1,gameSound[14],0);
				}else{
					//if only hit not killed!
					Mix_PlayChannel (-1,gameSound[rand() % 4],0);
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

    sprintf(buffer, "%d", score);
    string out = inputText + ": " + buffer + "\n";
    
    ofstream f;
    f.open ("highscore.txt", std::ios_base::app);
    f << out;
    f.close();
}

void reset(){
    resetWorld();
}
