#ifndef __structs_DEF__
#define __structs_DEF__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct {
	long length;
	char *payload;
} line_t;

typedef struct {
	long length;
	char *data;
	char **lines;
	long linecount;
} buffer_t;

typedef struct {
	int x;
	int y;
} cursor_t;

typedef struct {
	char *filepath;
	buffer_t *buffer; // The buffer content (contains the data of the file and the lines.)
	int ccol; // Cursor Column
	int crow; // Cursor Row
	char **argv;
	bool isNewFile;
} context_t;

#endif