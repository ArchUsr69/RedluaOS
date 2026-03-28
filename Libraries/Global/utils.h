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

#endif
