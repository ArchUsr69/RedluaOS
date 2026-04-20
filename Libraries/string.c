#include <types.h>
#include <string.h>
#include <utils.h>

/*
-> This is a little helper that makes a string for you,
   so you don't have to manually manipulate the struct;
-> capacity means how much space to reserve for the string;
-> a capacity value of 0 will mean that you only reserve the length of the string,
   so you basically don't reserve extra space;
*/

string stringNew(char *text, size_t capacity) {
    size_t length = textLength(text);
    capacity = (capacity == 0 || capacity < length) ? length : capacity;

    for (uint32 offset = length + 1; offset < capacity; offset++) {
        text[offset] = 0;
    }
    
    return (string){
        .length = length,
        .capacity = capacity,
        .text = text
    };
}

// ------------------------ //

/*
-> calculates the length of the string;
-> is mostly not needed, as stringNew calculates the string length for you;
-> but if you have to manipulate strings by yourself, then feel free to use it.
*/

size_t textLength(char *text) {
    size_t length = 0;
    while (text[length] != '\0') length++;
    return length;
}

// ------------------------ //

/*
-> copies one string literal to another;
-> just overwrites data, so be carefull;
*/

void stringCopy(string *target, string source, size_t length) {
    target->length = 0;

    for (uint32 offset = 0; offset < length && offset < target->capacity; offset++) {
        target->text[offset] = source.text[offset];
        target->length++;
    }

    target->text[(target->length + 1)] = '\0';
}

// ------------------------- //

/*
-> makes a new string from 2 other strings;
*/

string stringCombine(string source1, string source2) {
    char text[(source1.capacity + source2.capacity)];
    uint32 textOffset = 0;

    for (size_t offset = 0; offset < source1.length; offset++) {
        text[textOffset++] = source1.text[offset];
    }

    for (size_t offset = 0; offset < source2.length; offset++) {
        text[textOffset++] = source2.text[offset];
    }
    
    text[textOffset + 1] = '\0';
    return (string){
        .length = source1.length + source2.length,
        .capacity = sizeof(text),
        .text = text
    };
}

// ------------------------ //

bool stringCompare(string source1, string source2, size_t length) {
    if (length > source1.length) length = source1.length;
    if (length > source2.length) length = source2.length;

    for (size_t offset = 0; offset < length; offset++) {
        if (source1.text[offset] != source2.text[offset]) return true;
    }
    return false;
}

/*
-> translate interpreted numbers into fixed sized string literals;
-> they translate a 32-bit number;
*/

char *bin2text(uint32 number) {
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

char *hex2text(uint32 number) {
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

// ----------------------- //
