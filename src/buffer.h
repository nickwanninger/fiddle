#ifndef __file_DEF__
#define __file_DEF__

#include <stdlib.h>
#include <stdio.h>
#include "structs.h"


buffer_t buffer_new();
void buffer_setcontent(buffer_t *b, char *data, long len);
int buffer_loadfile(buffer_t *b, char *path);
void buffer_updatelines(buffer_t *b);
void buffer_updatedata(buffer_t *b);


#endif