#include <stdint.h>
#include <stdbool.h>
#include <mailbox.h>
#include <lowLevel.h>
#include <gpio.h>

#ifndef BCM2835_H
#define BCM2835_H

//==========GPIO==========//

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector register base;
#define GPIOFSELECT_BASE ((volatile uint32_t *)(GPIO_BASE))

// Output set Registers
#define GPIOSET0 ((volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((volatile uint32_t *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((volatile uint32_t *)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((volatile uint32_t *)(GPIO_BASE + 0x2C))

// ACT_LED and PWR_LED gpio pin number
#define ACT_LED 47
#define PWR_LED 35

// Misc
#define TOTAL_PINS 53
#define REGISTER_SIZE 32

/*
-> maps the standard gpio function numbers to the bcm2835 gpio function values
*/

static const uint8_t bcm2835gpio_functions[8] = {
    0b000, // Input
    0b001, // Output
    0b100, // Alternative 0
    0b101, // Alternative 1
    0b110, // Alternative 2
    0b111, // Alternative 3
    0b011, // Alternative 4
    0b010, // Alternative 5
};

// ------------------------- //

// Function prototypes
void bcm2835gpio_setFunction(uint8_t pin, enum gpioFunctions function);
void bcm2835gpio_pinWrite(uint8_t pin, bool level);

#endif
