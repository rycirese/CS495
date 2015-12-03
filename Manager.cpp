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
    bool done = false;
    SDL_Event event;
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
    
    ALLSYSTEMSGO(); //Sets Everything Up

    m = true; //Menu Mode is on (Loads Menu Not Game
    while(!done){
        const Uint8* keyState = SDL_GetKeyboardState(NULL); //Record Keystate
        if(!m) player->control(keyState); //Manage Player Controls
        if(keyState[SDL_SCANCODE_S]) m = false; //Turn Menu Mode Off (Start Game)
        if(keyState[SDL_SCANCODE_Q]){ done = true; quit(); break; }
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){ //Closes Everything Appropriately
                done = true;
                quit();
                break;
            }
        }
        draw(keyState); //Draws Everything
    }
    return 0;
}

void ALLSYSTEMSGO(){
    genWorld();
    genMenu();
    player = new Player();
	//for(int i=0;i<10;i++){ monsters[i]=new Monster();monsters[i]=NULL;} //monsters dont exist, only allocated
	createMonster(1,1,1);
    

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
		//Mix_PlayChannel (-1,mDeath,0);
	}
}

void draw(const Uint8* keyState){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear The Screen And The Depth Buffer
    glColor4f( 1.0f, 1.0f, 1.0f, 0.0f);
    glLoadIdentity();
    
	if(m) drawMenu(keyState);
    if(!m){
		player->draw();
		//draw monster
        for(int i=0;i<10;i++){
			if(monsters[i]!=NULL){
				//all this gibberish just says moves monster towards player and draws
				// if ( MONSTERS_X < PLAYERS_X ) MONSTERS_X += MONSTERS_SPEED
				// else MONSTERS_X -= MONSTERS_SPEED
				GLfloat Mx=monsters[i]->getX();
				GLfloat Mz=monsters[i]->getZ();
				GLfloat Ms=monsters[i]->getSpeed(); 

				GLfloat Px=player->getX();
				GLfloat Pz=player->getZ();

				if(Mx<Px) monsters[i]->setX(Mx+Ms);
				else monsters[i]->setX(Mx-Ms); 
				if(Mz<Pz) monsters[i]->setZ(Mz+Ms);
				else monsters[i]->setZ(Mz-Ms);
				monsters[i]->setY(player->getY());
				monsters[i]->draw();

			}
        }
		drawWorld(window);
	}
	SDL_GL_SwapWindow(window);
}

void quit(){
    quitWorld();
}
