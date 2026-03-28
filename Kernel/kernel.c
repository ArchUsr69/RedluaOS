#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ----------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>
#include <utils.h>
#include <broadcom.h>

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(47, HIGH);
    gpioPinWrite(35, LOW);

    uint16_t totalCharacters = GlobalConsole.columns * GlobalConsole.rows;

    while (true) {
        for (uint16_t i = 0; i < totalCharacters; i++) {
            consoleWrite(Red, Background, 70);
        }

        GlobalConsole.cursorX = 0;
        GlobalConsole.cursorY = 0;

        for (uint16_t i = 0; i < totalCharacters; i++) {
            consoleWrite(Blue, Background, 70);
        }

        GlobalConsole.cursorX = 0;
        GlobalConsole.cursorY = 0;
    }
}
