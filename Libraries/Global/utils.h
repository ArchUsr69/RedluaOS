#include <stdint.h>
#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

#define HIGH 1U
#define LOW 0

#define BYTE_SIZE 8
#define WORD_SIZE 16
#define DWORD_SIZE 32

// Specially made for MMIO Registers, but can be used for normal RAM Usage;
typedef volatile uint8_t REGISTER_8;
typedef volatile uint16_t REGISTER_16;
typedef volatile uint32_t REGISTER_32;

/*
-> functions that switches the bit at a selected offset;
-> the offset is the indexed bit which you want to change;
*/

static inline void switchBit8(REGISTER_8 *target, uint8_t offset) {
    if (offset >= BYTE_SIZE) return;
    *target ^= (HIGH << offset);
}

static inline void switchBit16(REGISTER_16 *target, uint8_t offset) {
    if (offset >= WORD_SIZE) return;
    *target ^= (HIGH << offset);
}

static inline void switchBit32(REGISTER_32 *target, uint8_t offset) {
    if (offset >= DWORD_SIZE) return;
    *target ^= (HIGH << offset);
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

static inline void writeField8(REGISTER_8 *target, uint8_t value, uint8_t offset, uint8_t length) {
    if (offset + length >= BYTE_SIZE) return;
    uint8_t mask = ((HIGH << length) - HIGH) << offset;
    *target &= ~mask;
    *target |= (value << offset) & mask;
}

static inline void writeField16(REGISTER_16 *target, uint16_t value, uint8_t offset, uint8_t length) {
    if (offset + length >= WORD_SIZE) return;
    uint16_t mask = ((HIGH << length) - HIGH) << offset;
    *target &= ~mask;
    *target |= (value << offset) & mask;
}

static inline void writeField32(REGISTER_32 *target, uint32_t value, uint8_t offset, uint8_t length) {
    if (offset + length >= DWORD_SIZE) return;
    uint32_t mask = ((HIGH << length) - HIGH) << offset;
    *target &= ~mask;
    *target |= (value << offset) & mask;
}

// ------------------------------ //


/*
-> functions that return the selected Bit (offset), pretty easy to understand, right?
*/

static inline bool readBit8(REGISTER_8 *target, uint8_t offset) {
    if (offset >= BYTE_SIZE) return LOW;
    return (*target >> offset) & HIGH;
}

static inline bool readBit16(REGISTER_16 *target, uint8_t offset) {
    if (offset >= WORD_SIZE) return LOW;
    return (*target >> offset) & HIGH;
}

static inline bool readBit32(REGISTER_32 *target, uint8_t offset) {
    if (offset >= DWORD_SIZE) return LOW;
    return (*target >> offset) & HIGH;
}

// ------------------------------ //


/*
-> functions that return a selected bit field, with offset being the starting point and length the desired field to read;
-> offset and length function the same as in the functions from above;
*/

static inline uint8_t readField8(REGISTER_8 *target, uint8_t offset, uint8_t length) {
    if (offset + length >= BYTE_SIZE) return LOW;
    return (*target >> offset) & ((HIGH << length) - HIGH);
}

static inline uint16_t readField16(REGISTER_16 *target, uint8_t offset, uint8_t length) {
    if (offset + length >= WORD_SIZE) return LOW;
    return (*target >> offset) & ((HIGH << length) - HIGH);
}

static inline uint32_t readField32(REGISTER_32 *target, uint8_t offset, uint8_t length) {
    if (offset + length >= DWORD_SIZE) return LOW;
    return (*target >> offset) & ((HIGH << length) - HIGH);
}

// ------------------------------ //

#endif
