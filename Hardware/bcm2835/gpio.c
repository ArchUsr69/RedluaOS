#include <stdint.h>
#include <stdbool.h>
#include "bcm2835.h"
#include "../../Libraries/lowLevel.h"

// basically just like the Arduino version of pinMode()
void gpio_setFunction(uint8_t pin, uint8_t function) {
    uint8_t start = (pin % 10) * 3;
    volatile uint32_t *target = GPIOFSELECT_BASE + (pin / 10);
    writeField32(target, function, start, 3);
}

// and this one just the Arduino version of digitalWrite(pin, OUTPUT)
void gpio_pinWrite(uint8_t pin, bool level) {
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
            writeBit32(GPIOCLEAR0, pin - 32, HIGH);
        }
    }
}
