#include <types.h>
#include <utils.h>
#include <broadcom.h>
#include <gpio.h>

#define GPIO_BASE (PERIPHERAL_BASE + 0x200000)

// GPIO Pin Function selector registers
#define GPIOFSELECT_BASE ((MMIO_32)(GPIO_BASE + 0x00))

// Output set Registers
#define GPIOSET0 ((MMIO_32)(GPIO_BASE + 0x1C))
#define GPIOSET1 ((MMIO_32)(GPIO_BASE + 0x20))

// Output clear Registers
#define GPIOCLEAR0 ((MMIO_32)(GPIO_BASE + 0x28))
#define GPIOCLEAR1 ((MMIO_32)(GPIO_BASE + 0x2C))

#define TOTAL_PINS (53)

/*
-> maps the standard gpio function numbers to the bcm2835 gpio function values;
-> put into an array for easy indexing;
-> Those are the raw values that will be written to the Function Selet registers;
*/

static const uint8 BCMgpioFunctions[8] = {
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
   In order to not overwrite the other pins with 0, it creates a mask; feel free to analyze it;
*/

void BCMgpioSetFunction(uint8 pin, enum gpioFunctions function) {
    if (pin > TOTAL_PINS) return;
    GPIOFSELECT_BASE[pin / 10] &= ~(0b111 << ((pin % 10) * 3));
    GPIOFSELECT_BASE[pin / 10] |= BCMgpioFunctions[function] << ((pin % 10) * 3);
}

// ------------------------- //

/*
-> and this one just the Arduino version of digitalWrite();

   To set a pin, you must write HIGH to the corresponding bit inside the SET registers;
   For clearing a pin, you must write HIGH to the corresponding bit inside the CLEAR registers;
   writing LOW to the SET/CLEAR registers won't do anything;
*/

void BCMgpioPinWrite(uint8 pin, bool level) {
    if (pin > TOTAL_PINS) return;
    if (level == HIGH) {
        MMIO_32 target = (pin <= INT32_WIDTH) ? GPIOSET0 : GPIOSET1;
        *target = (HIGH << (pin % INT32_WIDTH));
    } else {
        MMIO_32 target = (pin <= INT32_WIDTH) ? GPIOCLEAR0 : GPIOCLEAR1;
        *target = (HIGH << (pin % INT32_WIDTH));
    }
}

// ------------------------- //
