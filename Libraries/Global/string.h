#include <types.h>

#ifndef STRING_H
#define STRING_H

typedef char *string;

typedef struct {
    char string[40];
} wrappedString;

size_t stringLength(string text);
wrappedString stringCombine(const string source1, const string source2);

wrappedString bin2text(uint32 number);
wrappedString hex2text(uint32 number);

#endif
