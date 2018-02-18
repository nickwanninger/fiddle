#include "view.h"
#include "config.h"
#include <string.h>
#include <ncurses.h>

void initcodeview() {
	codeview = newwin(LINES - 1, COLS, 0, 0);
	init_pair(90, COLOR_WHITE, COLOR_BLACK);
	wbkgd(codeview, COLOR_PAIR(90));
	keypad(codeview, true);
	// leaveok(codeview, true);
	scrollok(codeview, true);
	wrefresh(codeview);
}


void initinfoview() {
	infoview = newwin(1, COLS, LINES - 1, 0);
	init_pair(91, COLOR_BLACK, COLOR_WHITE);
	wbkgd(infoview, COLOR_PAIR(91));
	// keypad(infoview, true);
	wrefresh(infoview);
}

void viewinit() {
	mainview = initscr();

	cbreak();
	noecho();
	keypad(stdscr, true);
	
	start_color();
	refresh();
	initcodeview();
	initinfoview();
}


void infoviewrefresh(context_t c) {

}


void viewresized() {
}


void drawcode(context_t *ctx) {
	
	long offset = ctx->scrolloffset;
	long linecount = ctx->buffer->linecount;

	for (int i = 0; i < LINES - 1; i++) {
		mvprintw(i, 0, "");
		clrtoeol();
	}

	int lc = 0;
	for(int i = offset; lc < LINES-1; i++) {
		if(!(i >= linecount)) {
			char* line = ctx->buffer->lines[i];
			int linelen = strlen(ctx->buffer->lines[i]);
			int x = 0;
			for (int c = 0; c < linelen; c++) {
				if (line[c] == 0x09) {
					x += TAB_WIDTH;
				} else {
					mvprintw(lc, x++, &line[c]);
				}
			}
		}
		clrtoeol();
		lc++;
	}
}