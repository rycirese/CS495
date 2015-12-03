#include "Menu.h"

TTF_Font* title;
TTF_Font* font;
TTF_Font* font2;
string menuCode;
GLuint mTexture[1];

void genMenu(){
    title =TTF_OpenFont("data/fonts/mrsmonsteracad.ttf", 100);
    font = TTF_OpenFont("data/fonts/Arial.ttf", 50);
    font2 = TTF_OpenFont("data/fonts/Arial.ttf", 35);
    menuCode = "menu";
    
    SDL_Surface **TI;
    TI = new SDL_Surface *[1];
    TI[0] = IMG_Load("data/textures/Menu.jpg");
    
    glGenTextures(0, &mTexture[0]); //Create The Texture
    glBindTexture(GL_TEXTURE_2D, mTexture[0]); //Load in texture 1
    glTexImage2D(GL_TEXTURE_2D, 0, 3, TI[0]->w, TI[0]->h, 0, FORMAT, GL_UNSIGNED_BYTE, TI[0]->pixels); //Generate The Texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    SDL_FreeSurface(TI[0]);
}

void drawMenu(const Uint8* keyState){
    glEnable2D();
    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, mTexture[0]);
    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); glVertex3d(0, 720, 0);
        glTexCoord2d(1, 1); glVertex3d(1280, 720, 0);
        glTexCoord2d(1, 0); glVertex3d(1280, 0, 0);
        glTexCoord2d(0, 0); glVertex3d(0, 0, 0);
    glEnd();
    
    glColor3f(0, 0, 0);
    glRenderText(title, 0, 0, 0, 365, 560, "Monster Game");
    if(keyState[SDL_SCANCODE_M]) menuCode = "menu";
    if(menuCode == "menu") menu();
    if(keyState[SDL_SCANCODE_H]) menuCode = "highscores";
    if(menuCode == "highscores") highscores();
    if(keyState[SDL_SCANCODE_C]) menuCode = "controls";
    if(menuCode == "controls") controls();
    
    glDisable2D();
}

void menu(){
    glRenderText(font, 0, 0, 0, 560, 450, "Menu");
    glRenderText(font2, 0, 0, 0, 365, 410, "'S' - Start");
    glRenderText(font2, 0, 0, 0, 365, 365, "'C' - Controls");
    glRenderText(font2, 0, 0, 0, 365, 320, "'H' - Highscores");
}

void controls(){
    glRenderText(font, 0, 0, 0, 540, 450, "Controls");
    glRenderText(font2, 0, 0, 0, 365, 410, "'^' - Move Forward");
    glRenderText(font2, 0, 0, 0, 365, 365, "'v' - Move Backward");
    glRenderText(font2, 0, 0, 0, 365, 320, "'<' - Turn Left");
    glRenderText(font2, 0, 0, 0, 365, 275, "'>' - Turn Right");
    glRenderText(font2, 0, 0, 0, 365, 200, "'A' - Strafe Left");
    glRenderText(font2, 0, 0, 0, 365, 155, "'D' - Strafe Right");
    glRenderText(font2, 0, 0, 0, 365, 110, "'Spacebar' - Shoot");
    glRenderText(font2, 0, 0, 0, 50, 50, "'M' - Menu");
    glRenderText(font2, 0, 0, 0, 250, 50, "'S' - Start");
    glRenderText(font2, 0, 0, 0, 430, 50, "'H' - Highscores");
}

void highscores(){
    glRenderText(font, 0, 0, 0, 500, 450, "Highscores");
    glRenderText(font2, 0, 0, 0, 50, 50, "'M' - Menu");
    glRenderText(font2, 0, 0, 0, 250, 50, "'S' - Start");
    glRenderText(font2, 0, 0, 0, 430, 50, "'H' - Highscores");
}
