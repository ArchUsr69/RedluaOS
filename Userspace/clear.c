#include <redlib.h>

#include <Kernel/console.h>

void new_line();
void redConsole();

void clear() {
    for (uint16 y = 0; y < Console.rows; y++) {
        for (uint16 x = 0; x < Console.columns; x++) {
            consoleWriteCharXY(Background, Background, ' ', x, y);
        }
    }

    new_line();
    Console.cursorX = 0;
    Console.cursorY = 0;
    redConsole();
}