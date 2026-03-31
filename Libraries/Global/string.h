#include <types.h>

#ifndef STRING_H
#define STRING_H

typedef struct {
    uint32 length;
    uint32 capacity;
    char *text;
} string;

string newString(char *literal, uint32 capacity);
uint32 stringLength(char *literal);

#endif
