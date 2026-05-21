#include <redlib.h>

/*
-> calculates the length of a string;
*/

size_t stringLength(string text) {
    size_t length = 0;
    while (text[length] != '\0') length++;
    return length;
}

// ------------------------ //

/*
-> makes a new string from 2 other strings;
-> can only work with strings below 32 characters;
-> mostly unusable with longer text until i can use the heap;
*/

wrappedString stringCombine(const string source1, const string source2) {
    wrappedString result;

    size_t length1 = stringLength(source1);
    size_t length2 = stringLength(source2);

    size_t stringOffset = 0;

    for (size_t offset = 0; offset < length1 && offset < 40; offset++) {
        result.string[stringOffset++] = source1[offset];
    }

    for (size_t offset = 0; offset < length2 && offset < 40; offset++) {
        result.string[stringOffset++] = source2[offset];
    }
    
    result.string[stringOffset] = '\0';

    return result;
}

// ------------------------ //

/*
-> translate interpreted numbers into fixed sized string literals;
-> they translate a 32-bit number;
*/

wrappedString bin2text(const uint32 number) {
    wrappedString binNumber;

    for (uint8 offset = 0; offset < 32; offset++) {
        bool bit = (number >> (31 - offset)) & HIGH;
        binNumber.string[offset + 2] = (bit) ? '1' : '0';
    }

    binNumber.string[0] = '0';
    binNumber.string[1] = 'b';
    binNumber.string[32] = '\0';

    return binNumber;
}

wrappedString hex2text(const uint32 number) {
    wrappedString hexNumber;

    for (uint8 offset = 0; offset < 10; offset++) {
        uint8 hex = (number >> ((7 - offset) * 4)) & 0xF;
        hexNumber.string[offset + 2] = (hex < 10) ? ('0' + hex) : ('A' + (hex - 10));
    }

    hexNumber.string[0] = '0';
    hexNumber.string[1] = 'x';
    hexNumber.string[10] = '\0';

    return hexNumber;
}

// ----------------------- //
