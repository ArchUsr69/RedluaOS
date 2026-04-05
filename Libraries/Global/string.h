#include <types.h>

#ifndef STRING_H
#define STRING_H

typedef struct {
    size_t length;
    size_t capacity;
    char *text;
} string;

string stringNew(char *literal, size_t capacity);
size_t stringLength(char *literal);
char *bin2string(uint32 number);
char *hex2string(uint32 number);

#endif
