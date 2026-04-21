#include <gpio.h>
#include <framebuffer.h>
#include <console.h>
#include <uart.h>

void kernelMain();

#ifdef Broadcom
    #include <broadcom.h>

    struct gpio Gpio = {
        .setFunction = BCMgpioSetFunction,
    };

    struct framebuffer Framebuffer = {
        .init = BCMframebufferInit,
        .virtualHeight = 720,
        .virtualWidth = 1280,
        .physicalHeight = 720,
        .physicalWidth = 1280,
        .virtual_X_Offset = 0,
        .virtual_Y_Offset = 0,
        .depth = 16,
        .pixelOrder = 1
    };

    struct console Console = {0};

#endif

void armv8Init() {
    Framebuffer.init();
    consoleInit();
    kernelMain();
}
