#include <redlib.h>
#include <Kernel/gpio.h>
#include <Kernel/framebuffer.h>
#include <Kernel/uart.h>
#include <Kernel/console.h>

#ifdef Broadcom
    #include <Drivers/broadcom.h>

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
