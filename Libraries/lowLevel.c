#include <stdint.h>
#include <stddef.h>
#include "lowLevel.h"

//----------Bitwise-helpers---------//
// sets a bit to 1 at a specified offset (1 Byte Number)
void bit8_setOn(volatile uint8_t *target, uint8_t offset) {
  *target |= (1 << offset);
}

// same before but for a 2 Byte number
void bit16_setOn(volatile uint16_t *target, uint16_t offset) {
  *target |= (1 << offset);
}

// and this one for a 4 Byte Number
void bit32_setOn(volatile uint32_t *target, uint32_t offset) {
  *target |= (1 << offset);
}

// sets a bit to 0 at a specified offset (4 Byte Number)
void bit32_setOff(volatile uint32_t *target, uint32_t offset) {
  *target &= ~(1 << offset);
}

// writes to a section of 1 Byte number of specified value and length with the starting point start
void bit8_setSection(volatile uint8_t *target, uint8_t value, uint8_t start, uint8_t length) {
    uint8_t mask = ((1 << length) - 1) << start;
    *target &= ~mask;
    *target |= (value << start) & mask;
}

// same as before, but with a 2 Byte number
void bit16_setSection(volatile uint16_t *target, uint16_t value, uint16_t start, uint16_t length) {
    uint16_t mask = ((1 << length) - 1) << start;
    *target &= ~mask;
    *target |= (value << start) & mask;
}

// this one takes a 4 Byte number
void bit32_setSection(volatile uint32_t *target, uint32_t value, uint32_t start, uint32_t length) {
    uint32_t mask = ((1 << length) - 1) << start;
    *target &= ~mask;
    *target |= (value << start) & mask;
}
