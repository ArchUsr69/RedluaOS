#include <stdint.h>
#include <stdbool.h>
// -------------- //
#include <gpio.h>

#ifndef BCM2835_H
#define BCM2835_H

// ACT_LED and PWR_LED gpio pin number
#define STATUS_LED 47
#define POWER_LED 35

void bcm2835gpioSetFunction(uint8_t pin, enum gpioFunctions function);
void bcm2835gpioPinWrite(uint8_t pin, bool level);

#endif
