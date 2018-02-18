#include "buffer.h"
#include <errno.h>
#include "config.h"
#include <ncurses.h>


buffer_t buffer_new() {
	buffer_t n = {};
	n.data = "";
	return n;
}



void buffer_setcontent(buffer_t *b, char *data, long len) {
	printf("got here");
	b->length = len;
	b->data = data;
	buffer_updatelines(b);
}


int buffer_loadfile(buffer_t *b, char *path) {
	
	FILE *fp;
	fp = fopen(path, "r");
	
	if (fp == NULL) {
		fprintf(stderr, "Error opening file: %s\n", strerror( errno ));
		return 0;
	}
	
	// Seek to the end of the file
	fseek(fp, 0, SEEK_END);
	// Get the size of the file based on that
	// new location gained from seeking
	b->length = ftell(fp);
	
	// And rewind the file read pointer
	rewind(fp);
	
	// Allocate enough memory for that new data
	b->data = calloc(b->length + 1, 1);
	
	// Read from the file and put it into the buffer
	fread(b->data, b->length, 1, fp);
	
	// content.file = fp;
	fclose(fp);
	

	// Convert the buffer's string data into lines that the editor can use.
	buffer_updatelines(b);
	
	return 1;
}

int buffer_writefile(buffer_t *buf, char *path) {
	FILE *f = fopen(path, "w");
	if (f == NULL) {
		return 0;
	}
	// Do a last minute update
	buffer_updatedata(buf);
	// printf("saving");
	fprintf(f, "%s", buf->data);
	fclose(f);
	return 1;
}


// Implement string dupe, since its kinda non-standard, or something
char* strdup_(const char * s) {
	size_t len = 1+strlen(s);
	char *p = malloc(len);
	// If p exist, return the coppied data; otherwise, return NULL
	// We return null in case there is an error in what string was passed
	// into the function.
	return p ? memcpy(p, s, len) : NULL;
}


void buffer_updatelines(buffer_t *buf) {
	
	char** lines = malloc(sizeof(char*));
	
	int linecount = 0;
	char* bufferdata = strdup_(buf->data);
	
	char *currentline = strtoke(bufferdata, LINE_DELIM);
	
	while(currentline) {
		lines = realloc(lines, (linecount + 1) * sizeof(char**));
		lines[linecount] = strdup_(currentline);
		currentline = strtoke(NULL, LINE_DELIM);
		linecount++;
	}
	buf->linecount = linecount;
	buf->lines = lines;
}


// Moves the buffer's lines to the raw string data.
void buffer_updatedata(buffer_t *buf) {
	char *data = malloc(sizeof(char));

	
	data[0] = 0;
	long totalsize = 0;
	
	int ll;
	int i;
	for (i = 0; i < buf->linecount; i++) {
		// The length of the current line
		ll = strlen(buf->lines[i]);
		
		// Increase the total size of the string
		totalsize += ll;
		// 
		data = realloc(data, strlen(data) + ll + 1);
		
		// printf("%s\n", data);
		
		strcat(data, buf->lines[i]);
		strcat(data, "\n");
	}
	buf->length = totalsize;
	buf->data = data;
}


/* 
 * behaves like strtok() except that it returns empty tokens also
 * From: https://stackoverflow.com/a/42315689
 * Thanks, scheff :)
 */
char* strtoke(char *str, const char *delim) {
  static char *start = NULL; /* stores string str for consecutive calls */
  char *token = NULL; /* found token */
  /* assign new start in case */
  if (str) start = str;
  /* check whether text to parse left */
  if (!start) return NULL;
  /* remember current start as found token */
  token = start;
  /* find next occurrence of delim */
  start = strpbrk(start, delim);
  /* replace delim with terminator and move start to follower */
  if (start) *start++ = '\0';
  /* done */
  return token;
}


// This function is a little strange, but it is required to get the
// correct x position to render the cursor at if the line contains
// tab characters since I am rendering tabs as spaces.
// This means moving over a tab character will shift the cursor
// 4 spaces to the right.
int buffer_getrendercolumn(char *line, int col) {
	int x = 0;
	int c;
	for (c = 0; c < col; c++) {
		if (line[c] == 0x09) {
			x += TAB_WIDTH;
		} else {
			x++;
		}
	}
	return x;
}

int buffer_getcolumnfromrendercolumn(char *line, int rcol) {
	int x = 0;
	int c;
	for (c = 0; c < rcol; c++) {
		if (line[c] == 0x09) {
			if (x + TAB_WIDTH > rcol) {
				return x;
			} else {
				x += TAB_WIDTH;
			}
		} else {
			x++;
		}
	}
	return x;
}