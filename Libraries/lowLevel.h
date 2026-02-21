#include <stdint.h>
#include <stddef.h>

#ifndef LOWLEVEL_H
#define LOWLEVEL_H

//----------Bitwise-helpers---------//

void bit8_setOn(volatile uint8_t *target, uint8_t offset);
void bit16_setOn(volatile uint16_t *target, uint16_t offset);
void bit32_setOn(volatile uint32_t *target, uint32_t offset);

void bit32_setOff(volatile uint32_t *target, uint32_t offset);

void bit8_setSection(volatile uint8_t *target, uint8_t value, uint8_t start, uint8_t length);
void bit16_setSection(volatile uint16_t *target, uint16_t value, uint16_t start, uint16_t length);
void bit32_setSection(volatile uint32_t *target, uint32_t value, uint32_t start, uint32_t length);

#endif
