#include "buffer.h"
#include <errno.h>


buffer_t buffer_new() {
	buffer_t n = {};
	n.data = malloc(1);
	return n;
}



void buffer_setcontent(buffer_t *b, char *data, long len) {
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
	return 1;
}



char * strdup(const char * s)
{
  size_t len = 1+strlen(s);
  char *p = malloc(len);

  return p ? memcpy(p, s, len) : NULL;
}





void buffer_updatelines(buffer_t *buf) {
	char** lines = malloc(sizeof(char*));
	int linecount = 0;
	char* bufferdata = strdup(buf->data);

	char *currentline = strtoke(bufferdata, LINE_DELIM);
	while(currentline) {
		lines = (char**)realloc(lines, (linecount + 1) * sizeof(char*));
		lines[linecount] = strdup(currentline);
		currentline = strtoke(NULL, LINE_DELIM);
		linecount++;
	}
	buf->linecount = linecount;
	buf->lines = lines;
}


// Moves the buffer's lines to the raw string data.
void buffer_updatedata(buffer_t *buf) {
	buf->data = malloc(1);

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