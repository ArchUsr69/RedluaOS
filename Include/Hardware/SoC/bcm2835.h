#include <stdint.h>
#include <stdbool.h>

#ifndef BCM2835_H
#define BCM2835_H

// ACT_LED and PWR_LED gpio pin number
#define ACT_LED 47
#define PWR_LED 35

extern enum gpioFunctions function;

void bcm2835gpio_setFunction(uint8_t pin, enum gpioFunctions function);
void bcm2835gpio_pinWrite(uint8_t pin, bool level);

#endif
