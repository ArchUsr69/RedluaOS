#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <gpio.h>
#include <lowLevel.h>

/*
-> basically just like the Arduino version of pinMode();
-> can only take pin number 0-53, since only those pins exist on the board;
-> translates the standard gpio function numbers into the bcm2835 gpio function values;
*/ 

void bcm2835gpio_setFunction(uint8_t pin, enum gpio_functions function) {
    if (pin > TOTAL_PINS) return;
    uint8_t start = (pin % 10) * 3;
    writeField32(GPIOFSELECT_BASE + (pin /10), bcm2835gpio_functions[function], start, 3);
}

// ------------------------- //

/* 
-> and this one just the Arduino version of digitalWrite();
-> can only take pin number 0-53;
*/

void bcm2835gpio_pinWrite(uint8_t pin, bool level) {
    if (pin > TOTAL_PINS) return;
    if (level) {
        if (pin < REGISTER_SIZE) {
            writeBit32(GPIOSET0, pin, HIGH);
        } else {
            writeBit32(GPIOSET1, pin - REGISTER_SIZE, HIGH);
        }
    } else {
        if (pin < REGISTER_SIZE) {
            writeBit32(GPIOCLEAR0, pin, HIGH);
        } else {
            writeBit32(GPIOCLEAR1, pin - REGISTER_SIZE, HIGH);
        }
    }
}

// ------------------------- //
