#include "main.h"
#include "buffer.h"
#include "view.h"
int main(int argc, char** argv) {
	char* targetfile = *(argv + (argc - 1)); // argv[argc - 1];

	context_t maincontext;
	buffer_t mainbuffer = buffer_new();

	maincontext.buffer = &mainbuffer;
	maincontext.filepath = targetfile;

	if (!buffer_loadfile(&mainbuffer, targetfile)) {
		printf("FILE NOT FOUND");
		return 0;
	}
	

	maincontext.cursor.x = 0;
	maincontext.cursor.y = 0;
	
	// Initialize the ncurses views and windows
	viewinit();
	
	// The variable that will store each key as a user presses it.
	unsigned int c;
	while (1) {
		refresh();
		if (maincontext.cursor.y < 0) maincontext.cursor.y = 0;
		if (maincontext.cursor.x < 0) maincontext.cursor.x = 0;

		
		wmove(codeview, maincontext.cursor.y, maincontext.cursor.x);
		wrefresh(codeview);
		
		c = getch();

		switch (c) {
		case KEY_UP:
			maincontext.cursor.y--;
			break;
		case KEY_DOWN:
			maincontext.cursor.y++;
			break;
		case KEY_LEFT:
			maincontext.cursor.x--;
			break;
		case KEY_RIGHT:
			maincontext.cursor.x++;
			break;
		case KEY_RESIZE:
			viewresized();
		case 0x0a:
			maincontext.cursor.x = 0;
			maincontext.cursor.y++;
			break;
		default:
			wprintw(codeview, "%c", c);
			maincontext.cursor.x++;
			// wrefresh(codeview);
		}
	}

	endwin();

	return 0;

}