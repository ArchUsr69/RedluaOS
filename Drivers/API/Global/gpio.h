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
    void (*setFunction)(uint16 pin, enum gpioFunctions function);
    void (*pinWrite)(uint16 pin, bool level);
    uint16 totalPins;
};

extern struct gpio Gpio;

#endif
