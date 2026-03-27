#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ----------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>
#include <utils.h>

struct framebuffer framebuffer = {
    .virtualHeight = 1080,
    .virtualWidth = 1920,
    .physicalHeight = 1080,
    .physicalWidth = 1920,
    .virtual_X_Offset = 0,
    .virtual_Y_Offset = 0,
    .depth = 16
};

struct console console = {0};

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(15, OUTPUT);
    gpioPinWrite(47, LOW);
    gpioPinWrite(15, LOW);

    uint16_t totalCharacters = console.columns * console.rows;

    while (true) {
        for (uint16_t i = 0; i < totalCharacters; i++) {
            consoleWrite(Red, Background, 70);
        }

        console.cursorX = 0;
        console.cursorY = 0;

        for (uint16_t i = 0; i < totalCharacters; i++) {
            consoleWrite(Blue, Background, 70);
        }

        console.cursorX = 0;
        console.cursorY = 0;
    }
}
