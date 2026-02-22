#include <stdint.h>
#include <stdbool.h>

#ifndef LOWLEVEL_H
#define LOWLEVEL_H

//==========Bitwise-helpers==========//

void bit8_setOn(volatile uint8_t *target, uint8_t offset);
void bit16_setOn(volatile uint16_t *target, uint16_t offset);
void bit32_setOn(volatile uint32_t *target, uint32_t offset);

void bit8_setOff(volatile uint8_t *target, uint8_t offset);
void bit16_setOff(volatile uint16_t *target, uint16_t offset);
void bit32_setOff(volatile uint32_t *target, uint32_t offset);

void bit8_setSection(volatile uint8_t *target, uint8_t value, uint8_t start, uint8_t length);
void bit16_setSection(volatile uint16_t *target, uint16_t value, uint16_t start, uint16_t length);
void bit32_setSection(volatile uint32_t *target, uint32_t value, uint32_t start, uint32_t length);

bool bit8_readBit(volatile uint8_t *target, uint8_t offset);
bool bit16_readBit(volatile uint16_t *target, uint16_t offset);
bool bit32_readBit(volatile uint32_t *target, uint32_t offset);

uint8_t bit8_readSection(volatile uint8_t *target, uint8_t offset, uint8_t length);
uint16_t bit16_readSection(volatile uint16_t *target, uint16_t offset, uint16_t length);
uint32_t bit32_readSection(volatile uint32_t *target, uint32_t offset, uint32_t length);

#endif
