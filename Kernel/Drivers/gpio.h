#include <stdint.h>
#include <stdbool.h>

#ifndef GPIO_H
#define GPIO_H

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

// Function table
struct gpioTable {
    void (*setFunction)(uint8_t pin, enum gpioFunctions function);
    void (*pinWrite)(uint8_t pin, bool level);
};

// pointer to global struct instance (defined by main)
extern struct gpioTable *gpio;

// Wrapper functions
static inline void gpio_setFunction(uint8_t pin, enum gpioFunctions function) {
    gpio->setFunction(pin, function);
}

static inline void gpio_pinWrite(uint8_t pin, bool level) {
    gpio->pinWrite(pin, level);
}


#endif
