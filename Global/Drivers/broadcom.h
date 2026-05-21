#include <redlib.h>
#include <Kernel/gpio.h>

#ifndef BROADCOM_H
#define BROADCOM_H

#ifdef BCM2835
    #define PERIPHERAL_BASE 0x20000000
#endif

#ifdef BCM2712
    #define PERIPHERAL_BASE 0x107C000000
    #define RP1_BASE 0x1F00000000
#endif

// GPIO functions;
void BCMgpioSetFunction(uint16 pin, enum gpioFunctions function);
void BCMgpioPinWrite(uint16 pin, bool level);
void BCMgpioDisablePullup(uint16 pin);

// framebuffer functions;
void BCMframebufferInit();

// UART functions;
void BCMuartInit();
void BCMuartWriteByte(char byte);
char BCMuartReadByte();
char BCMuartReadByte_NI();
void BCMuartWriteText(string string);

#endif
