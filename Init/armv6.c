#include <stdint.h>
#include <stddef.h>
// --------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <utils.h>
#include <console.h>
#include <broadcom.h>

// Preprocessor bullshit
#ifdef BROADCOM
    #include <broadcom.h>
    struct gpioTable gpioTable = {
        .setFunction = BCMgpioSetFunction,
        .pinWrite = BCMgpioPinWrite
    };

    struct framebufferTable framebufferTable = {
        .framebufferInit = BCMframebufferInit
    };
#endif

void kernelMain();

void armv6Init() {
    BCMframebufferInit();
    consoleInit();
    kernelMain();
}
