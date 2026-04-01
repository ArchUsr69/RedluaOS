#include <types.h>
#include <string.h>

string newString(char *literal, uint32 capacity) {
    uint32 literalLength = stringLength(literal);
    return (string){
        .length = literalLength,
        .capacity = (capacity == 0) ? literalLength : capacity,
        .text = literal
    };
}

uint32 stringLength(char *literal) {
    uint32 length = 0;
    while (literal[length] != '\0') length++;
    return length;
}
