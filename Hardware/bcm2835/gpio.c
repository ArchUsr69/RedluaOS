#include <stdint.h>
#include <stdbool.h>
// ---------------- //
#include <bcm2835.h>
#include <gpio.h>
#include <utils.h>

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector register base
#define GPIOFSELECT_BASE ((volatile uint32_t *)(GPIO_BASE))

// Output set Registers
#define GPIOSET0 ((volatile uint32_t *)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((volatile uint32_t *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((volatile uint32_t *)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((volatile uint32_t *)(GPIO_BASE + 0x2C))

// Misc
#define TOTAL_PINS 53
#define REGISTER_SIZE 32

/*
-> maps the standard gpio function numbers to the bcm2835 gpio function values;
-> put into an array for easy indexing;
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


/*
-> basically just like the Arduino version of pinMode();
-> can only take pin number 0-53, since only those pins exist on the board;
-> translates the standard gpio function numbers into the bcm2835 gpio function values;
*/

void bcm2835gpio_setFunction(uint8_t pin, enum gpioFunctions function) {
    if (pin > TOTAL_PINS) return;
    uint8_t start = (pin % 10) * 3;
    writeField32(GPIOFSELECT_BASE + (pin / 10), bcm2835gpio_functions[function], start, 3);
}

// ------------------------- //

/*
-> and this one just the Arduino version of digitalWrite();
-> can only take pin number 0-53;
*/

void bcm2835gpio_pinWrite(uint8_t pin, bool level) {
    if (pin > TOTAL_PINS) return;
    if (level) {
        volatile uint32_t *register = (pin <= REGISTER_SIZE) ? GPIOSET0 : GPIOSET1;
        writeBit32(register, pin % REGISTER_SIZE, HIGH);
    } else {
        volatile uint32_t *register = (pin <= REGISTER_SIZE) ? GPIOCLEAR0 : GPIOCLEAR1;
        writeBit32(register, pin % REGISTER_SIZE, HIGH);
    }
}

// ------------------------- //
