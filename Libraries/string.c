#include <types.h>
#include <string.h>

string newString(char *literal, uint32 capacity) {
    return (string){
        .length = stringLength(literal),
        .capacity = (capacity == 0) ? stringLength(literal) : capacity,
        .text = literal
    };
}

uint32 stringLength(char *literal) {
    uint32 counter = 0;
    while  (literal[counter] != '\0') {
        counter++;
    }
    return counter;
}
