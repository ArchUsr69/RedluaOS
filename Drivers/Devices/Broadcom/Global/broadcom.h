#include <stdint.h>
#include <stdbool.h>
// --------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <utils.h>

#ifndef BROADCOM_H
#define BROADCOM_H

const REGISTER_32 PERIPHERAL_BASE;

void BCMgpioSetFunction(uint8_t pin, enum gpioFunctions function);
void BCMgpioPinWrite(uint8_t pin, bool level);
void BCMframebufferInit();

#endif