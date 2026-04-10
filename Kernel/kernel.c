#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <framebuffer.h>
#include <uart.h>
#include <console.h>

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);

    string prompt = stringNew(">", 0);
    string message = stringNew("[RedluaOS] UART ready.\n", 0);
    
    uint16 totalCharacters = GlobalConsole.rows * GlobalConsole.columns;

    while (true) {
        for (size_t i = 0; i < totalCharacters; i++) {
            consoleWrite(Blue, Background, prompt);
            uartWriteText(message);
        }

        GlobalConsole.cursorY = 0;
        GlobalConsole.cursorX = 0;

        for (size_t i = 0; i < totalCharacters; i++) {
            consoleWrite(Red, Background, prompt);
        }

        GlobalConsole.cursorY = 0;
        GlobalConsole.cursorX = 0;
    }
}
