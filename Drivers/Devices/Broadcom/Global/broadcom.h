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

// framebuffer functions;
void BCMframebufferInit();

// UART functions;
void BCMuartInit();
void BCMuartWriteByte(char byte);
char BCMuartReadByte();
void BCMuartWriteText(string string);

#endif
