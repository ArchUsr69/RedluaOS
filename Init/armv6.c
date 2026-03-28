#include <stdint.h>
#include <stddef.h>
// --------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <utils.h>
#include <console.h>
#include <broadcom.h>

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

// It is declared here too so that it can call Main;
void kernelMain();

struct framebufferInfo GlobalFramebuffer = {
    .virtualHeight = 1080,
    .virtualWidth = 1920,
    .physicalHeight = 1080,
    .physicalWidth = 1920,
    .virtual_X_Offset = 0,
    .virtual_Y_Offset = 0,
    .depth = 16
};

struct consoleInfo GlobalConsole = {0};

void armv6Init() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);
    framebufferInit();
    consoleInit();
    kernelMain();
}
