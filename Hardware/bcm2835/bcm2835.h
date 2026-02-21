#include <stdint.h>
#include "../../Libraries/lowLevel.h"

#ifndef BCM2835_H
#define BCM2835_H

// A Library which stores every specific aspect of the BCM2835 Hardware in one place

//==========GPIO==========//

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector
#define GPIOFSELECT_BASE ((volatile uint32_t *)(GPIO_BASE))
#define GPIOFSELECT_OFFSET 0x04

// GPIO Functions
#define INPUT 0b000
#define OUTPUT 0b001
#define FUNCTION_0 0b100
#define FUNCTION_1 0b101
#define FUNCTION_2 0b110
#define FUNCTION_3 0b111
#define FUNCTION_4 0b011
#define FUNCTION_5 0b010

// Output set Registers
#define GPIOSET0 ((volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((volatile uint32_t *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((volatile uint32_t *)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((volatile uint32_t *)(GPIO_BASE + 0x2C))

// Functions
void gpio_setFunction(uint8_t pin, uint8_t function);
void gpio_pinOutput(uint8_t pin);

//==========Mailbox==========//

#define MAILBOX_BASE 0x2000B88

// Mailbox Registers
#define MAILBOX_READ ((volatile unsigned int *)(MAILBOX_BASE + 0x00))
#define MAILBOX_STATUS ((volatile unsigned int *)(MAILBOX_BASE + 0x18))
#define MAILBOX_WRITE ((volatile unsigned int *)(MAILBOX_BASE + 0x20))

#endif
