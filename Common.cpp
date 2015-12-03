//Methods that are commonly used in more
//than one place are stored here for easy
//accessing
#include "Common.h"


void glRenderText(TTF_Font *font, int r, int g, int b, int x, int y, string text){    
    SDL_Color color = {static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b)};
    SDL_Surface *tmp = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    SDL_Surface *msg = SDL_CreateRGBSurface(SDL_SWSURFACE, tmp->w, tmp->h, 32,	0x000000FF,	0x0000FF00, 0x00FF0000, 0xFF000000);
    
    SDL_Rect area;
    area.x = area.y = 0;
    area.w = tmp->w;
    area.h = tmp->h;
    
    SDL_BlitSurface(tmp, &area, msg, &area);
    
    GLuint texture[1];
    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, msg->w, msg->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, msg->pixels);
    
    glBegin(GL_QUADS);
        glTexCoord2d(0, 0); glVertex2d(x, y+msg->h);
        glTexCoord2d(0, 1); glVertex2d(x, y);
        glTexCoord2d(1, 1); glVertex2d(x+msg->w, y);
        glTexCoord2d(1, 0); glVertex2d(x+msg->w, y+msg->h);
    glEnd();
	glDeleteTextures(1,&texture[0]);
}

void glEnable2D(){
    int vPort[4];
    glGetIntegerv(GL_VIEWPORT, vPort);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, vPort[2], 0, vPort[3], -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void glDisable2D(){
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
