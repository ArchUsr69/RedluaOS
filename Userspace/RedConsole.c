#include <types.h>
#include <utils.h>
#include <string.h>
#include <console.h>
#include <uart.h>

char buffer[50];
size_t index = 0;

char waitForInput() {
    for (size_t cycles = 35000; cycles > 0; cycles--) {
        char value = uartReadByte();
        if (value != 0) return value;

        consoleWriteCharXY(Foreground, Background, '_', Console.cursorX, Console.cursorY);
    }
    for (size_t cycles = 35000; cycles > 0; cycles--) {
        char value = uartReadByte();
        if (value != 0) return value;

        consoleWriteCharXY(Background, Background, '_', Console.cursorX, Console.cursorY);
    }
}

void newLine() {
    Console.cursorX = 0;
    Console.cursorY++;
}

void commandParser() {
    index = 0;
    string echo = stringNew(buffer, 0);
    consoleWrite(Foreground, Background, echo);
    memorySet(buffer, 0, sizeof(buffer));
    newLine();
}

void RedConsole() {
    string prompt = stringNew("$ ", 0);
    memorySet(buffer, 0, sizeof(buffer));

    consoleWrite(Red, Background, prompt);

    while (1) {
        char input = waitForInput();

        if (input == '\r') {
            consoleWriteChar(Background, Background, ' ');
            newLine();
            commandParser();

            consoleWrite(Red, Background, prompt);
        }

        if (input == '\b' && Console.cursorX > prompt.length) {
            buffer[--index] = 0;
            consoleWriteCharXY(Background, Background, ' ', Console.cursorX, Console.cursorY);

            Console.cursorX--;
            consoleWriteCharXY(Background, Background, ' ', Console.cursorX, Console.cursorY);

        } else if ((input > 32 || input == ' ') && index < sizeof(buffer)) {

            consoleWriteChar(Foreground, Background, input);
            buffer[index++] = input;
        }
    }
}
