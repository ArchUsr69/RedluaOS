#include <types.h>
#include <utils.h>
#include <gpio.h>

void RedConsole();

void kernelMain() {
    Gpio.setFunction(47, OUTPUT);
    Gpio.setFunction(35, OUTPUT);
    Gpio.pinWrite(35, LOW);
    Gpio.pinWrite(47, HIGH);

    RedConsole();
}
