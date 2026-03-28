#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ----------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>
#include <utils.h>


// Fills the screen with F; Just try it out;
void kernelMain() {
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
