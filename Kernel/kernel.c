#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ----------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>
#include <utils.h>

struct gpioTable gpioTable;
struct framebufferTable framebufferTable;

struct framebuffer framebuffer;
struct console console;

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(15, OUTPUT);
    gpioPinWrite(47, LOW);
    gpioPinWrite(15, LOW);

    framebufferInit();
    consoleInit();

    uint16_t totalCharacters = console.columns * console.rows;

    while (true) {
        for (uint16_t i = 0; i < totalCharacters; i++) {
            consoleWrite(OneDarker.red, 70);
        }

        console.cursorX = 0;
        console.cursorY = 0;

        for (uint16_t i = 0; i < totalCharacters; i++) {
            consoleWrite(OneDarker.blue, 70);
        }

        console.cursorX = 0;
        console.cursorY = 0;
    }
}
