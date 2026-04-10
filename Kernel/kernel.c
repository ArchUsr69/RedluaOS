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

    uint16 totalCharacters = GlobalConsole.rows * GlobalConsole.columns;

    while (true) {
        uartWriteByte(68);
        char lol = uartReadByte();
        string message = stringNew(hex2text((uint8)lol), 0);
        consoleWrite(Blue, Background, message);

        GlobalConsole.cursorX = 0;
        GlobalConsole.cursorY = 0;
    }
}
