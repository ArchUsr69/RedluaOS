#include <redlib.h>

#include <Kernel/console.h>
#include <Kernel/uart.h>

void snake();

char InputBuffer[40];
size_t Index = 0;

void new_line() {
    Index = 0;
    memorySet(InputBuffer, 0, sizeof(InputBuffer));

    Console.cursorX = 0;
    Console.cursorY++;
}

void parseCommand() {
    if (memoryCompare(InputBuffer, "snake", 5) == 0) {
        snake();
    } else {
        new_line();
        consoleWrite(Foreground, Background, "This command doesn't exist. Check for typos");
        return;
    }
}

/*
-> a non blocking way to check for input;
-> looks pretty cool with the blinking cursor;
-> blinking speed depends on cycles and clock speed;
*/

char waitForInput() {
    for (size_t cycles = 18000; cycles > 0; cycles--) {
        char input = Uart.readByte();
        if (input != 0) return input;
        consoleWriteCharXY(Foreground, Background, '_', Console.cursorX, Console.cursorY);
    }

    for (size_t cycles = 18000; cycles > 0; cycles--) {
        char input = Uart.readByte();
        if (input != 0) return input;
        consoleWriteCharXY(Background, Background, '_', Console.cursorX, Console.cursorY);
    }
}

// ------------------------------ //

void redConsole() {
    consoleWrite(Red, Background, "$ ");

    while (true) {
        char input = waitForInput();

        if (input == '\r') {
            parseCommand();
            new_line();
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

