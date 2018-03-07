#include "main.h"
#include "buffer.h"
#include "view.h"
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
	context_t ctx;

	buffer_t mainbuffer = buffer_new();

	ctx.argv = argv;
	ctx.buffer = &mainbuffer;
	ctx.filepath = targetfile;
	ctx.y = 0;
	ctx.x = 0;


	// Load the file into the buffer if the file is not found, 
	// set a flag in the context that says the file needs to be
	// created
	if (!buffer_loadfile(&mainbuffer, ctx.filepath)) {
		// The file was not found, so a file needs to be created.
		mainbuffer.data = malloc(0);
		ctx.isNewFile = true;
	} else {
		// The file was found, so a new file doesnt need to be created.
		ctx.isNewFile = false;
	}



	// Set the second line to some new value
	// mainbuffer.lines[0] = "hello world";
	// Compile the lines back into one single string
	buffer_updatedata(&mainbuffer);


	// return 0;
	// Initialize the ncurses views and windows
	viewinit();

	// The variable that will store each key as a user presses it.
	unsigned int c;
	while (1) {
		buffer_refresh(&mainbuffer);
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
				buffer_writefile(&mainbuffer, ctx.filepath);
			case KEYCODE_NL:
				ctx.x = 0;
				// ctx.y++;
				break;
			case 0x11: // the quit function
				endwin();
				return 0;
				break;
			default:
				handleinput(&ctx, c);
				// if (c >= 32 && c <= 127) {
				// 	wprintw(codeview, "%c", c);
				// 	ctx.x++;
				// } else {
				// 	// The char is not printable, so ring the bell. :)
				// 	// printf("\a");
				// }
				break;
		}

	}


	endwin();

	return 0;
}
