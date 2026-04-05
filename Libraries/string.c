#include <types.h>
#include <string.h>
#include <utils.h>

/*
-> This is a little helper that makes a string for you,
   so you don't have to manually manipulate the struct;
-> capacity means how much space to reserve for the string;
-> a capacity value of 0 will mean that you only reserve the length of the string;
*/

string stringNew(char *literal, size_t capacity) {
    size_t literalLength = stringLength(literal);
    return (string){
        .length = literalLength,
        .capacity = (capacity == 0 || capacity < literalLength) ? literalLength : capacity,
        .text = literal
    };
}

// ------------------------ //

/*
-> calculates the length of the string;
-> is mostly not needed, as stringNew calculates the string length for you;
*/

size_t stringLength(char *literal) {
    size_t length = 0;
    while (literal[length] != '\0') length++;
    return length;
}

// ------------------------ //

/*
-> Turns a number in binary format into a string;
-> upon error will return NULL, keep that in mind;
*/

char *bin2string(uint32 number) {
    static char binNumber[33];
    for (uint8 offset = 0; offset < 32; offset++) {
        bool bit = (number >> (31 - offset)) & HIGH;
        binNumber[offset + 2] = (bit) ? '1' : '0';
    }
    binNumber[0] = '0';
    binNumber[1] = 'b';
    binNumber[32] = '\0';
    return binNumber;
}

// ------------------------- //

/*
-> Turns a HEX number to a string;
-> upon error will return NULL;
*/

char *hex2string(uint32 number) {
    static char hexNumber[11];
    for (uint8 offset = 0; offset < 10; offset++) {
        uint8 hex = (number >> ((7 - offset) * 4)) & 0xF;
        hexNumber[offset + 2] = (hex < 10) ? ('0' + hex) : ('A' + (hex - 10));
    }
    hexNumber[0] = '0';
    hexNumber[1] = 'x';
    hexNumber[10] = '\0';
    return hexNumber;
}

// ------------------------- //
