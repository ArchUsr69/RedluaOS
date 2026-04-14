#include <types.h>
#include <utils.h>
#include <gpio.h>

void RedConsole();

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);

    RedConsole();
}
