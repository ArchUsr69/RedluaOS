#include <gpio.h>
#include <framebuffer.h>
#include <console.h>

void kernelMain();

// Preprocessor bullshit
#ifdef Broadcom
    #include <broadcom.h>
    
    struct gpioTable GlobalGpioTable = {
        .setFunction = BCMgpioSetFunction,
        .pinWrite = BCMgpioPinWrite
    };

    struct framebufferTable GlobalFramebufferTable = {
        .framebufferInit = BCMframebufferInit
    };
#endif

struct framebufferInfo GlobalFramebuffer = {
    .virtualHeight = 720,
    .virtualWidth = 1280,
    .physicalHeight = 720,
    .physicalWidth = 1280,
    .virtual_X_Offset = 0,
    .virtual_Y_Offset = 0,
    .depth = 16,
    .pixelOrder = 1
};

struct consoleInfo GlobalConsole = {0};

void armv6Init() {
    framebufferInit();
    consoleInit();
    kernelMain();
}
