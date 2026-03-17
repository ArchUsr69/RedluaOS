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

// pointer to global struct instance (defined by main for now)
extern struct gpioTable *gpio;

// Wrapper functions
static inline void gpio_setFunction(uint8_t pin, enum gpioFunctions function) {
    gpio->setFunction(pin, function);
}

static inline void gpio_pinWrite(uint8_t pin, bool level) {
    gpio->pinWrite(pin, level);
}

#endif
