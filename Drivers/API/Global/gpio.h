#include <stdint.h>
#include <stdbool.h>

#ifndef GPIO_H
#define GPIO_H

/*
-> GPIO Functions that the kernel can see;
-> depending on physical Pin, the same function might do something different;
-> All GPIO Drivers must find a way to translate those functions locally;
-> for example see the bcm2835 gpio driver;
*/

enum gpioFunctions {
    INPUT,
    OUTPUT,
    ALTERNATIVE0,
    ALTERNATIVE1,
    ALTERNATIVE2,
    ALTERNATIVE3,
    ALTERNATIVE4,
    ALTERNATIVE5
};

// ------------------------- //

// Driver Table
struct gpioTable {
    void (*setFunction)(uint8_t pin, enum gpioFunctions function);
    void (*pinWrite)(uint8_t pin, bool level);
};

extern struct gpioTable GlobalGpioTable;

// Wrapper functions
static inline void gpioSetFunction(uint8_t pin, enum gpioFunctions function) {
    GlobalGpioTable.setFunction(pin, function);
}

static inline void gpioPinWrite(uint8_t pin, bool level) {
    GlobalGpioTable.pinWrite(pin, level);
}

#endif
