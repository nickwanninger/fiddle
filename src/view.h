#ifndef __view_DEF__
#define __view_DEF__

#include <ncurses.h>
#include "structs.h"

WINDOW *mainview;
WINDOW *codeview;
WINDOW *infoview;

int rows, cols;

void viewinit();
void drawchar(int x, int y, char c);
void typechar(char c);
void viewresized();

void infoviewrefresh(context_t c);



#endif