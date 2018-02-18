#ifndef __view_DEF__
#define __view_DEF__

#include <ncurses.h>
#include "structs.h"

WINDOW *mainview;
WINDOW *codeview;
WINDOW *infoview;

int rows, cols;

void viewinit();
void typechar(char c);
void viewresized();

void infoviewrefresh(context_t c);
void drawcode(context_t *ctx);


#endif