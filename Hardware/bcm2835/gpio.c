#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <gpio.h>
#include <lowLevel.h>

static const uint8_t bcm2835_gpio_functions[8] = {
    0b000, // Input
    0b001, // Output
    0b100, // Alternative 0
    0b101, // Alternative 1
    0b110, // Alternative 2
    0b111, // Alternative 3
    0b011, // Alternative 4
    0b010, // Alternative 5
};


/*
-> basically just like the Arduino version of pinMode();
-> can only take pin number 0-53, since only those pins exist on the board;
*/ 

void bcm2835gpio_setFunction(uint8_t pin, enum gpio_functions function) {
    if (pin > 53) return;
    uint8_t start = (pin % 10) * 3;
    writeField32(GPIOFSELECT_BASE + (pin /10), bcm2835_gpio_functions[function], start, 3);
}

// ------------------------- //

/* 
-> and this one just the Arduino version of digitalWrite();
-> can only take pin number 0-53;
*/

void bcm2835gpio_pinWrite(uint8_t pin, bool level) {
    if (pin > 53) return;
    if (level) {
        if (pin < 32) {
            writeBit32(GPIOSET0, pin, HIGH);
        } else {
            writeBit32(GPIOSET1, pin - 32, HIGH);
        }
    } else {
        if (pin < 32) {
            writeBit32(GPIOCLEAR0, pin, HIGH);
        } else {
            writeBit32(GPIOCLEAR1, pin - 32, HIGH);
        }
    }
}

// ------------------------- //
