#include <stdint.h>
#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

//==========Bitwise-helpers==========//

/*
-> functions that write 1/0 to a selected bit at a selected offset;
-> the offset is the indexed bit which you want to change;
-> bit is, uhh well, the value, duhhh (1 or 0);
*/

static inline void writeBit8(volatile uint8_t *target, uint8_t offset, bool bit) {
    if (offset >= 8) { /* does nothing; safety check */ return; }
    if (bit) {
        *target |= (1 << offset);
    } else {
        *target &= ~(1 << offset);
    }
}

static inline void writeBit16(volatile uint16_t *target, uint8_t offset, bool bit) {
    if (offset >= 16) { return; }
    if (bit) {
        *target |= (1 << offset);
    } else {
        *target &= ~(1 << offset);
    }
}

static inline void writeBit32(volatile uint32_t *target, uint8_t offset, bool bit) {
    if (offset >= 32) { return; }
    if (bit) {
        *target |= (1 << offset);
    } else {
        *target &= ~(1 << offset);
    }
}

// ------------------------------ //

/*
-> functions that write to specific sections of specified length, value and start;
-> written for different type sizes to avoid UB and other type problems;
-> value is well, the value you want to write to the target in binary form
-> always specify the length of the value you're passing;
-> the offset is just the starting point in the target you want to change;

*IMPORTANT*:
don't align the value; the start of the value is always at the first bit of the number you're passing;
*/

static inline void writeField8(volatile uint8_t *target, uint8_t value, uint8_t offset, uint8_t length) {
    if (offset + length >= 8) { /* does nothing; safety check */ return; }
    uint8_t mask = ((1 << length) - 1) << offset;
    *target &= ~mask;
    *target |= (value << offset) & mask;
}

static inline void writeField16(volatile uint16_t *target, uint16_t value, uint8_t offset, uint8_t length) {
    if (offset + length >= 16) { return; }
    uint16_t mask = ((1 << length) - 1) << offset;
    *target &= ~mask;
    *target |= (value << offset) & mask;
}

static inline void writeField32(volatile uint32_t *target, uint32_t value, uint8_t offset, uint8_t length) {
    if (offset + length >= 32) { return; }
    uint32_t mask = ((1 << length) - 1) << offset;
    *target &= ~mask;
    *target |= (value << offset) & mask;
}

// ------------------------------ //


/*
-> functions that return the selected Bit (offset), pretty easy to understand, right?
*/

static inline bool readBit8(volatile uint8_t *target, uint8_t offset) {
    if (offset >= 8) { return 0; }
    return (*target >> offset) & 1;
}

static inline bool readBit16(volatile uint16_t *target, uint8_t offset) {
    if (offset >= 16) { return 0; }
    return (*target >> offset) & 1;
}

static inline bool readBit32(volatile uint32_t *target, uint8_t offset) {
    if (offset >= 32) { return 0; }
    return (*target >> offset) & 1;
}

// ------------------------------ //


/*
-> functions that return a selected bit field, with offset being the starting point and length the desired field to read;
-> offset and length function the same as in the functions from above;
*/

static inline uint8_t readField8(volatile uint8_t *target, uint8_t offset, uint8_t length) {
    if (offset + length >= 8) { return 0; }
    return (*target >> offset) & ((1 << length) - 1);
}

static inline uint16_t readField16(volatile uint16_t *target, uint8_t offset, uint8_t length) {
    if (offset + length >= 16) { return 0; }
    return (*target >> offset) & ((1 << length) - 1);
}

static inline uint32_t readField32(volatile uint32_t *target, uint8_t offset, uint8_t length) {
    if (offset + length >= 32) { return 0; }
    return (*target >> offset) & ((1 << length) - 1);
}

// ------------------------------ //

#endif
