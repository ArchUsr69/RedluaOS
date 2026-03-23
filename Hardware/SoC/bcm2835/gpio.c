#include <stdint.h>
#include <stdbool.h>
// ---------------- //
#include <bcm2835.h>
#include <gpio.h>
#include <utils.h>

#define GPIO_BASE 0x20200000

// GPIO Pin Function selector registers
#define GPIOFSELECT_BASE ((REGISTER_32 *)(GPIO_BASE + 0x00))

// Output set Registers
#define GPIOSET0 ((REGISTER_32 *)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((REGISTER_32 *)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((REGISTER_32 *)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((REGISTER_32 *)(GPIO_BASE + 0x2C))

#define TOTAL_PINS 53

/*
-> maps the standard gpio function numbers to the bcm2835 gpio function values;
-> put into an array for easy indexing;
-> Those are the raw values that will be written to the Function Selet registers;
*/

static const uint8_t bcm2835gpioFunctions[8] = {
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

   Essentially for each pin, there are 3 bits available for each pin;
   There are multiple Registers; so the offset must be calculated;
*/

void bcm2835gpioSetFunction(uint8_t pin, enum gpioFunctions function) {
    if (pin > TOTAL_PINS) return;
    uint8_t start = (pin % 10) * 3;
    writeField32(GPIOFSELECT_BASE + (pin / 10), bcm2835gpioFunctions[function], start, 3);
}

// ------------------------- //

/*
-> and this one just the Arduino version of digitalWrite();

   To set a pin, you must write HIGH to the corresponding bit inside the SET registers;
   For clearing a pin, you must write HIGH to the corresponding bit inside the CLEAR registers;
   writing LOW to the SET/CLEAR registers won't do anything;
*/

void bcm2835gpioPinWrite(uint8_t pin, bool level) {
    if (pin > TOTAL_PINS) return;
    if (level == HIGH) {
        REGISTER_32 *target = (pin <= LONG_SIZE) ? GPIOSET0 : GPIOSET1;
        switchBit32(target, pin % LONG_SIZE);
    } else {
        REGISTER_32 *target = (pin <= LONG_SIZE) ? GPIOCLEAR0 : GPIOCLEAR1;
        switchBit32(target, pin % LONG_SIZE);
    }
}

// ------------------------- //
