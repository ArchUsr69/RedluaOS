#include <types.h>
#include <utils.h>
#include <string.h>
#include <console.h>
#include <uart.h>

char InputBuffer[40];
size_t Index = 0;

void newLine() {
    Index = 0;
    memorySet(InputBuffer, 0, sizeof(InputBuffer));

    Console.cursorX = 0;
    Console.cursorY++;
}

/*
-> a non blocking way to check for input;
-> looks pretty cool with the blinking cursor;
-> blinking speed depends on cycles and clock speed;
*/

char waitForInput() {
    for (size_t cycles = 18000; cycles > 0; cycles--) {
        consoleWriteCharXY(Foreground, Background, '_', Console.cursorX, Console.cursorY);
    }

    for (size_t cycles = 18000; cycles > 0; cycles--) {
        consoleWriteCharXY(Background, Background, '_', Console.cursorX, Console.cursorY);
    }
}

// ------------------------------ //

void RedConsole() {
    consoleWrite(Red, Background, "$ ");

    while (true) {
        char input = waitForInput();

        if (input == '\r') {
            newLine();
            consoleWrite(Red, Background, "$ ");
        }

        if (input == '\b' && Console.cursorX > stringLength("$ ")) {
            InputBuffer[--Index] = 0;

            Console.cursorX--;
            consoleWriteCharXY(Background, Background, ' ', Console.cursorX, Console.cursorY);

        } else if ((input > 32 || input == ' ') && Index < sizeof(InputBuffer)) {

            consoleWriteChar(Foreground, Background, input);
            InputBuffer[Index++] = input;
        }
    }
}
