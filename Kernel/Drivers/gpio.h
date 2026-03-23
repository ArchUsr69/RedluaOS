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

// ---------------- //

/*
-> function table;
-> Must be filled with the driver equivalent at Init;
-> The Kernel only has access to this;
-> all Drivers must respect the types and arguments in order for them to works;
-> the struct MUST be global in order for the kernel to access it;
*/

struct gpioTable {
    void (*setFunction)(uint8_t pin, enum gpioFunctions function);
    void (*pinWrite)(uint8_t pin, bool level);
};

// ---------------- //

// Wrapper functions
static inline void gpioSetFunction(uint8_t pin, enum gpioFunctions function) {
    extern struct gpioTable *gpio;
    gpio->setFunction(pin, function);
}

static inline void gpioPinWrite(uint8_t pin, bool level) {
    extern struct gpioTable *gpio;
    gpio->pinWrite(pin, level);
}

#endif
