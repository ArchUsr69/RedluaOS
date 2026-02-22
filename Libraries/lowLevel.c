#include <stdint.h>
#include <stdbool.h>
#include "lowLevel.h"

//==========Bitwise-helpers==========//

// functions that write 1 to selected bits; for 1, 2 and 4 Byte numbers

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

//----------------------------//


// functions that write 0 to selected bits; for 1, 2 and 4 Byte numbers

void bit8_setOff(volatile uint8_t *target, uint8_t offset) {
    *target &= ~(1 << offset);
}


void bit16_setOff(volatile uint16_t *target, uint16_t offset) {
    *target &= ~(1 << offset);
}


void bit32_setOff(volatile uint32_t *target, uint32_t offset) {
    *target &= ~(1 << offset);
}

//-----------------------------//


// functions that write to specific sections of specified length, value and start, for 1, 2 and 4 Byte numbers

void bit8_setSection(volatile uint8_t *target, uint8_t value, uint8_t start, uint8_t length) {
    uint8_t mask = ((1 << length) - 1) << start;
    *target &= ~mask;
    *target |= (value << start) & mask;
}

void bit16_setSection(volatile uint16_t *target, uint16_t value, uint16_t start, uint16_t length) {
    uint16_t mask = ((1 << length) - 1) << start;
    *target &= ~mask;
    *target |= (value << start) & mask;
}

void bit32_setSection(volatile uint32_t *target, uint32_t value, uint32_t start, uint32_t length) {
    uint32_t mask = ((1 << length) - 1) << start;
    *target &= ~mask;
    *target |= (value << start) & mask;
}

//-----------------------------//


// functions that return a selected Bit for 1, 2  and 4 Byte numbers

bool bit8_readBit(volatile uint8_t *target, uint8_t offset) {
    return (*target >> offset) & 1;
}

bool bit16_readBit(volatile uint16_t *target, uint16_t offset) {
    return (*target >> offset) & 1;
}

bool bit32_readBit(volatile uint32_t *target, uint32_t offset) {
    return (*target >> offset) & 1;
}

//----------------------------//


// functions that return selected Bit Sections of selected length and offset, for 1, 2 and 4 Byte numbers

uint8_t bit8_readSection(volatile uint8_t *target, uint8_t offset, uint8_t length) {
    return (*target >> offset) & ((1 << length) - 1);
}

uint16_t bit16_readSection(volatile uint16_t *target, uint16_t offset, uint16_t length) {
    return (*target >> offset) & ((1 << length) - 1);
}

uint32_t bit32_readSection(volatile uint32_t *target, uint32_t offset, uint32_t length) {
    return (*target >> offset) & ((1 << length) - 1);
}

//---------------------------//
