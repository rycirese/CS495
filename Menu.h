//JOE BONINO & RYAN CIRESE
//Menu Header File
#ifndef Menu_h
#define Menu_h
#include "Common.h"

//Methods
void genMenu();
void drawMenu(bool gettingName, string inputText, const Uint8* keyState);
void name(string inputText);
void menu();
void highscores();
void controls();

#endif
