#include <stdint.h>
#include <stddef.h>
// --------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <utils.h>


// Preprocessor bullshit
#ifdef BROADCOM
    #include <broadcom.h>

    gpioTable.setFunction = BCMgpioSetFunction;
    gpioTable.pinWrite = BCMgpioPinWrite;

    framebufferTable.framebufferInit = BCMFramebufferInit;

    #ifdef BCM2835
        PERIPHERAL_BASE = 0x20000000;
    #endif
#endif

void armv6Init() {
    framebufferInit();
    consoleInit();
    extern kernelMain();
}