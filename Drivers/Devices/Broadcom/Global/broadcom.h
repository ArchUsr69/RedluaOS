#include <stdint.h>
#include <stdbool.h>
// --------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <utils.h>

#ifndef BROADCOM_H
#define BROADCOM_H

#ifdef BCM2835
    #define PERIPHERAL_BASE 0x20000000
#endif

void BCMgpioSetFunction(uint8_t pin, enum gpioFunctions function);
void BCMgpioPinWrite(uint8_t pin, bool level);
void BCMframebufferInit();

#endif
