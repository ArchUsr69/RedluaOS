#include <types.h>

#ifndef GPIO_H
#define GPIO_H

/*
-> GPIO Functions that the kernel can see;
-> depending on physical Pin, the same function might do something different;
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

struct gpio {
    void (*setFunction)(uint8 pin, enum gpioFunctions function);
    void (*pinWrite)(uint8 pin, bool level);
    size_t TotalPins;
};

extern struct gpio Gpio;

#endif
