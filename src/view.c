#include "view.h"

void initcodeview() {
	codeview = newwin(LINES - 1, COLS, 0, 0);
	init_pair(1, 78, 77);
	wbkgd(codeview, COLOR_PAIR(1));
	keypad(codeview, true);
	// leaveok(codeview, true);
	scrollok(codeview, true);
	wrefresh(codeview);
}


void initinfoview() {
	infoview = newwin(1, COLS, LINES - 1, 0);
	init_pair(2, 77, 78);
	wbkgd(infoview, COLOR_PAIR(2));
	// keypad(infoview, true);
	wrefresh(infoview);
}

void viewinit() {
	mainview = initscr();

	cbreak();
	noecho();
	keypad(stdscr, true);
	
	start_color();
	init_color(77, 133, 133, 133);
	init_color(78, 1000, 1000, 1000);
	// init_color(78, 1000, 1000, 1000);
	refresh();
	initcodeview();
	initinfoview();

	// getmaxyx(mainview,rows,cols);

}

void drawchar(int x, int y, char c) {
	mvprintw(y, x ,"%c", c);
}



void infoviewrefresh(context_t c) {

}


void viewresized() {
	// refresh();
	// // getmaxyx(mainview,LINES,COLS);
	
	// wresize(codeview, LINES - 1, COLS);
	// wrefresh(codeview);
}