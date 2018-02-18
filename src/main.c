#include "main.h"
#include "buffer.h"
#include "view.h"
#include <stdio.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include "keys.h"
 

int main(int argc, char** argv) {
	char* targetfile = argv[argc - 1];

	char *s;
	while (--argc > 0 && (*++argv)[0] == '-') {
		for(s = argv[0]+1; *s != '\0'; s++) {
			switch(*s) {
				case 'v':
					printf("fiddle v0.0.1");
					return 0;
					break;
				default:
					printf("Unknown Option '%c', Exiting...\n", *s);
					argc = 0;
					break;
			}
		}
	}
	if( argc != 1) {
		printf("Usage: fiddle <file>\n");
		return 0;
	}

	context_t maincontext;
	maincontext.argv = argv;
	buffer_t mainbuffer = buffer_new();

	maincontext.buffer = &mainbuffer;
	maincontext.filepath = targetfile;

	// Load the file into the buffer if the file is not found, 
	// set a flag in the context that says the file needs to be
	// created
	if (!buffer_loadfile(&mainbuffer, targetfile)) {
		// The file was not found, so a file needs to be created.
		maincontext.isNewFile = true;
	} else {
		// The file was found, so a new file doesnt need to be created.
		maincontext.isNewFile = false;
	}
	maincontext.ccol = 0;
	maincontext.crow = 0;
	
	// Build the buffer into lines
	buffer_updatelines(&mainbuffer);
	// Set the second line to some new value
	mainbuffer.lines[0] = "hello world";
	// Compile the lines back into one single string
	buffer_updatedata(&mainbuffer);

	// buffer_writefile(&mainbuffer, "out.example");


	// return 0;
	
	// Initialize the ncurses views and windows
	viewinit();

	// The variable that will store each key as a user presses it.
	unsigned int c;
	while (1) {
		refresh();
		if (maincontext.crow < 0) maincontext.crow = 0;
		if (maincontext.ccol < 0) maincontext.ccol = 0;
		if (maincontext.crow >= LINES - 1) maincontext.crow = LINES - 2;
		if (maincontext.ccol >= COLS) maincontext.ccol = COLS - 1;

		wmove(codeview, maincontext.crow, maincontext.ccol);
		wrefresh(codeview);
		
		c = getch();

		switch (c) {
			case KEY_RESIZE:
				viewinit();
			break;
			case KEY_UP:
				maincontext.crow--;
				break;
			case KEY_DOWN:
				maincontext.crow++;
				break;
			case KEY_LEFT:
				maincontext.ccol--;
				break;
			case KEY_RIGHT:
				maincontext.ccol++;
				break;

			case KEYCODE_SAVE:
				printf("need to save");
			case KEYCODE_NL:
				maincontext.ccol = 0;
				maincontext.crow++;
				break;
			default:

				if (c >= 32 && c <= 127) {
					wprintw(codeview, "%c", c);
					maincontext.ccol++;
				} else {
					wprintw(codeview, "%x", c);
					// The char is not printable, so ring the bell. :)
					printf("\a");
				}
				break;
		}
	}

	endwin();

	return 0;

}