#include "buffer.h"


buffer_t buffer_new() {
	buffer_t n = {};
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


void buffer_updatelines(buffer_t *buf) {

}

void buffer_updatedata(buffer_t *buf) {

}


