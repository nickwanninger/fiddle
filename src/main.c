#include "main.h"
#include "buffer.h"
#include "view.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <stdbool.h>
#include "keys.h"
 
#include <ncurses.h>

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

	context_t ctx;
	ctx.argv = argv;
	buffer_t mainbuffer = buffer_new();

	ctx.buffer = &mainbuffer;
	ctx.filepath = targetfile;

	// Load the file into the buffer if the file is not found, 
	// set a flag in the context that says the file needs to be
	// created
	if (!buffer_loadfile(&mainbuffer, targetfile)) {
		// The file was not found, so a file needs to be created.
		ctx.isNewFile = true;
	} else {
		// The file was found, so a new file doesnt need to be created.
		ctx.isNewFile = false;
	}
	ctx.y = 0;
	ctx.x = 0;
	// Set the second line to some new value
	mainbuffer.lines[0] = "hello world";
	// Compile the lines back into one single string
	buffer_updatedata(&mainbuffer);


	// char *test = "		tab";

	// int len = strlen(test);
	// for (int i = 0; i < len; i++) {
	// 	char c = test[i];
	// 	if (c == 0x09) {
	// 		for (int a = 0; a < 4; a++) {
	// 			printf("_");
	// 		}
	// 	} else {
	// 		printf("%c", c);
	// 	}
	// }

	// return 0;
	// Initialize the ncurses views and windows
	viewinit();

	// The variable that will store each key as a user presses it.
	unsigned int c;
	while (1) {
		
		drawcode(&ctx);
		refresh();
		int row = ctx.y + ctx.scrolloffset;
		char* currentline = "";
		if (row < mainbuffer.linecount) {
			currentline = mainbuffer.lines[row];
			if (ctx.x >= strlen(currentline)) {
				ctx.x = strlen(currentline);
			}
		} else {
			ctx.x = 0;
		}
		
		int rendercolumn = buffer_getrendercolumn(currentline, ctx.x);
		wmove(codeview, ctx.y, rendercolumn);
		
		wrefresh(codeview);
		
		
		c = getch();

		switch (c) {
			case KEY_RESIZE:
				viewinit();
			break;
			case KEY_UP:
				if (ctx.y > 0) {
					ctx.y--;
				} else if (ctx.y - 1 < 0 && ctx.scrolloffset > 0) {
					ctx.scrolloffset--;
				}
				
				break;
			case KEY_DOWN:
				if (ctx.y + 1 < LINES - 1 && ctx.y + 1 < mainbuffer.linecount) {
					ctx.y++;
				} else if (ctx.scrolloffset + ctx.y < mainbuffer.linecount) {
					ctx.scrolloffset++;
				}
				break;
			case KEY_LEFT:
				if (ctx.x - 1 >= 0) {
					ctx.x--;
				}
				break;
			case KEY_RIGHT:
				if (ctx.x + 1 >= 0) {
					ctx.x++;
				}
				break;

			case KEYCODE_SAVE:
				buffer_writefile(&mainbuffer, "out.example");
			case KEYCODE_NL:
				ctx.x = 0;
				ctx.y++;
				break;
			default:
				if (c >= 32 && c <= 127) {
					wprintw(codeview, "%c", c);
					ctx.x++;
				} else {
					// The char is not printable, so ring the bell. :)
					// printf("\a");
				}
				break;
		}
	}

	endwin();

	return 0;

}