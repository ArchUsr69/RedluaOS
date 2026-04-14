#include <gpio.h>
#include <framebuffer.h>
#include <uart.h>
#include <console.h>

void kernelMain();

// Preprocessor bullshit
#ifdef Broadcom
    #include <broadcom.h>
    
    struct gpioTable GpioTable = {
        .setFunction = BCMgpioSetFunction,
        .pinWrite = BCMgpioPinWrite
    };

    struct framebufferTable FramebufferTable = {
        .framebufferInit = BCMframebufferInit
    };

    struct uartTable UartTable = {
        .uartInit = BCMuartInit,
        .uartWriteByte = BCMuartWriteByte,
        .uartReadByte = BCMuartReadByte_NI,
        .uartWriteText = BCMuartWriteText
    };
#endif

struct framebufferInfo Framebuffer = {
    .virtualHeight = 720,
    .virtualWidth = 1280,
    .physicalHeight = 720,
    .physicalWidth = 1280,
    .virtual_X_Offset = 0,
    .virtual_Y_Offset = 0,
    .depth = 16,
    .pixelOrder = 1
};

struct consoleInfo Console = {0};

void armv6Init() {
    uartInit();
    framebufferInit();
    consoleInit();
    kernelMain();
}
