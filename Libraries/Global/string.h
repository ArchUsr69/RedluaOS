#include <types.h>

#ifndef STRING_H
#define STRING_H

typedef struct {
    size_t length;
    size_t capacity;
    char *text;
} string;

string stringNew(char *text, size_t capacity);
size_t textLength(char *text);
void stringCopy(string *target, string source, size_t length);
string stringCombine(string source1, string source2);

char *bin2text(uint32 number);
char *hex2text(uint32 number);

#endif
