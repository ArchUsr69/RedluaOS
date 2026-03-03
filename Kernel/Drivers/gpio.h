#include <stdint.h>
#include <stdbool.h>

#ifndef GPIO_H
#define GPIO_H

enum gpio_functions {
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
struct gpio_table {
    void (*setFunction)(uint8_t pin, enum gpio_functions function);
    void (*pinWrite)(uint8_t pin, bool level);
};

extern struct gpio_table *gpio;

// Wrapper functions
static inline void gpio_setFunction(uint8_t pin, enum gpio_functions function) {
    gpio->setFunction(pin, function);
}

static inline void gpio_pinWrite(uint8_t pin, bool level) {
    gpio->pinWrite(pin, level);
}


#endif
