#include <types.h>

#ifndef STRING_H
#define STRING_H

typedef struct {
    char *text;
} string;

typedef struct {
    size_t length;
    size_t capacity;
    string text;
} text;

text textNew(string text, size_t capacity);
size_t stringLength(string text);
void stringCopy(string *target, string source, size_t length);
string stringCombine(string source1, string source2);

string bin2text(uint32 number);
string hex2text(uint32 number);

#endif
