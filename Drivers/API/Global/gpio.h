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

// Driver Table
struct gpioTable {
    void (*setFunction)(uint8 pin, enum gpioFunctions function);
    void (*pinWrite)(uint8 pin, bool level);
};

extern struct gpioTable GlobalGpioTable;

// Wrapper functions
static inline void gpioSetFunction(uint8 pin, enum gpioFunctions function) {
    GlobalGpioTable.setFunction(pin, function);
}

static inline void gpioPinWrite(uint8 pin, bool level) {
    GlobalGpioTable.pinWrite(pin, level);
}

#endif
