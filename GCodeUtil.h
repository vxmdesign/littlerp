#ifndef GCODE_UTIL_H
#define GCODE_UTIL_H

#include <string.h>

int trimComment(char *x);
int iswhitespace(char c);
char *trim(char *x);
void strcpybase(char *dst, char *src, char delim);


#endif
