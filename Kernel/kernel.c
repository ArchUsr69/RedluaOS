#include <types.h>
#include <utils.h>
#include <gpio.h>
#include <framebuffer.h>
#include <uart.h>
#include <console.h>

#ifdef Broadcom
    #include <broadcom.h>

    struct framebuffer Framebuffer = {
        .init = BCMframebufferInit,
        .virtualHeight = 1080,
        .virtualWidth = 1920,
        .physicalHeight = 1080,
        .physicalWidth = 1920,
        .virtual_X_Offset = 0,
        .virtual_Y_Offset = 0,
        .depth = 16,
        .pixelOrder = 1
    };

    struct console Console = {0};

#endif

void RedConsole();

void kernelMain(){
    Framebuffer.init();
    consoleInit();
    RedConsole();
}
