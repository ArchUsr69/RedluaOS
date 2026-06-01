#include <redlib.h>
#include <Kernel/console.h>
#include <Kernel/framebuffer.h>
#include <Kernel/font.h>

/*
-> The default colour Palette RedluaOS will use;
-> Later on, i will make it possible to change the colours via user input;
*/

static uint16 ConsolePalette[18] = {
    0x0000, 0xDEFB, 0x2966, 0xDB6E, 0x7EED, 0xF7EC, 0x4D9E, 0xFB3D, 0x55B8,
    0xFFFF, 0xAD97, 0xDB6E, 0x7EED, 0xF7EC, 0x4D9E, 0xFB3D, 0x55B8, 0xFFFF
};

/*
-> Initializes a console;
-> MUST have the framebuffer initialized for it to work, or else it will just return;
*/

void consoleInit() {
    if (Framebuffer.pointer == 0 || Console.rows != 0) return;

    Console.rows = (Framebuffer.virtualHeight / CHARACTER_HEIGHT) + 1;
    Console.columns = Framebuffer.virtualWidth / CHARACTER_WIDTH;
    Console.cursorX = 0;
    Console.cursorY = 0;
};

// ---------------------- //

/*
-> this function manages the formatting and how the characters are written on the screen;
-> contains newline detection and coordinate calculation, stuff like that;
-> it must keep checking if the characters might be out of bounds, so performance might drop;
*/

void consoleWriteChar(enum consoleColours foreground, enum consoleColours background, char letter) {
    uint16 Foreground = ConsolePalette[foreground];
    uint16 Background = ConsolePalette[background];

    if (letter < 32 || Console.cursorY >= Console.rows) return;

    if (Console.cursorX >= Console.columns) {
        Console.cursorX = 0;
        Console.cursorY++;
    }

    renderBitmap(Foreground, Background, letter, CHARACTER_WIDTH, CHARACTER_HEIGHT, Console.cursorX, Console.cursorY);
    Console.cursorX++;
}

void consoleWriteCharXY(enum consoleColours foreground, enum consoleColours background, char letter, uint16 x, uint16 y) {
    uint16 Foreground = ConsolePalette[foreground];
    uint16 Background = ConsolePalette[background];

    if (letter < 32 || y >= Console.rows) return;

    if (x >= Console.columns) {
        x = 0;
        y++;
    }

    renderBitmap(Foreground, Background, letter, CHARACTER_WIDTH, CHARACTER_HEIGHT, x, y);
}

void consoleWrite(enum consoleColours foreground, enum consoleColours background, string text) {
    for (size_t index = 0; index < stringLength(text); index++) {
        consoleWriteChar(foreground, background, text[index]);
    }
}

void consoleWriteXY(enum consoleColours foreground, enum consoleColours background, string text, uint16 x, uint16 y) {
    for (size_t index = 0; index < stringLength(text); index++) {
        consoleWriteCharXY(foreground, background, text[index], x, y);
        x++;
    }
}

// ------------------------------------------------ //
