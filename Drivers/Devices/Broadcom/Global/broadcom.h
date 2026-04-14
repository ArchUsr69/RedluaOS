#include <types.h>
#include <gpio.h>
#include <string.h>

#ifndef BROADCOM_H
#define BROADCOM_H

#ifdef BCM2835
    #define PERIPHERAL_BASE 0x20000000
#endif

// GPIO functions;
void BCMgpioSetFunction(uint8 pin, enum gpioFunctions function);
void BCMgpioPinWrite(uint8 pin, bool level);
void BCMgpioDisablePullup(uint8 pin);

// framebuffer functions;
void BCMframebufferInit();

// UART functions;
void BCMuartInit();
void BCMuartWriteByte(char byte);
char BCMuartReadByte();
char BCMuartReadByte_NI();
void BCMuartWriteText(string string);

#endif
