#include <types.h>
#include <string.h>
#include <utils.h>

/*
-> This is a little helper that makes a string for you,
   so you don't have to manually manipulate the struct;
-> capacity means how much space to reserve for the string;
-> a capacity value of 0 will mean that you only reserve the length of the string;
*/

string stringNew(char *literal, uint32 capacity) {
    uint32 literalLength = stringLength(literal);
    return (string){
        .length = literalLength,
        .capacity = (capacity == 0) ? literalLength : capacity,
        .text = literal
    };
}

// ------------------------ //

/*
-> calculates the length of the string;
-> is mostly not needed, as stringNew calculates the string length for you;
*/

uint32 stringLength(char *literal) {
    uint32 length = 0;
    while (literal[length] != '\0') length++;
    return length;
}

// ------------------------ //

/*
-> Turns a number in binary format into a string;
-> upon error will return NULL, keep that in mind;
*/

char *bin2string(uint32 number, uint8 length) {
    if (length == 0 || length > INT32_WIDTH) return NULL;
    char *binNumber[length + 1];
    for (uint8 offset = length; offset > 0; offset--) {
        bool bit = ((number >> offset) & HIGH);
        binNumber[offset] = bit ? '1' : '0';
    }
    binNumber[length + 1] = '\0';
    return binNumber;
}

// ------------------------- //

/*
-> Turns a HEX number to a string;
-> upon error will return NULL;
*/

char *hex2string(uint32 number, uint8 length) {
    if (length == 0 || length > (INT32_WIDTH / 4)) return NULL;
    char *hexNumber[length];
    for (uint8 offset = length; offset > 0; offset--) {
        uint8 hex = ((number >> (offset * 4)) & 0xF);
        hexNumber[offset] = (hex < 10) ? ('0' + hex) : ('A' + (hex % 10));
    }
    hexNumber[length + 1] = '\0';
    return hexNumber;
}

// ------------------------- //
