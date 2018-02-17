#ifndef __structs_DEF__
#define __structs_DEF__

#include <stdint.h>
#include <stdlib.h>


typedef struct {
	long length;
	char *payload;
} line_t;

typedef struct {
	long length;
	char *data;
	line_t *lines;
} buffer_t;

typedef struct {
	int x;
	int y;
} cursor_t;

typedef struct {
	char *filepath;
	buffer_t *buffer;
	cursor_t cursor;
} context_t;

#endif