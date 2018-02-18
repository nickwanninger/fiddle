#include "view.h"

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

void drawchar(int x, int y, char c) {
	mvprintw(y, x ,"%c", c);
}


void infoviewrefresh(context_t c) {

}


void viewresized() {
}