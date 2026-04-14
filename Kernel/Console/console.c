#include <types.h>
#include <utils.h>
#include <string.h>
#include <framebuffer.h>
#include <console.h>
#include <font.h>

#define CHARACTER_WIDTH (8)
#define CHARACTER_HEIGHT (16)

/*
-> The default colour Palette RedluaOS will use;
-> Later on, i will make it possible to change the colours via user input;
*/

static uint16 ConsolePalette[18] = {
    0x0000,
    0xDEFB,
    0x2966,
    0xDB6E,
    0x7EED,
    0xF7EC,
    0x4D9E,
    0xFB3D,
    0x55B8,
    0xFFFF,
    0xAD97,
    0xDB6E,
    0x7EED,
    0xF7EC,
    0x4D9E,
    0xFB3D,
    0x55B8,
    0xFFFF
};

/*
-> Initializes a console;
-> MUST have the framebuffer initialized for it to work, or else it will just return;
*/

void consoleInit() {
    if (Framebuffer.pointer == 0 || Console.rows != 0) return;

    Console.rows = Framebuffer.virtualHeight / CHARACTER_HEIGHT;
    Console.columns = Framebuffer.virtualWidth / CHARACTER_WIDTH;
    Console.cursorX = 0;
    Console.cursorY = 0;
};

// ---------------------- //

/*
-> manages the rendering of characters;
-> It assumes depth = 16, so make sure that it is, or else it won't work;
-> It also uses the default Font Bitmap;
*/

void OPTIMIZE(3) renderCharacter(uint16 foreground, uint16 background, char character, uint16 Console_x, uint16 Console_y) {
    uint32 *screen = (uint32 *)Framebuffer.pointer;
    uint32 x = Console_x * (CHARACTER_WIDTH / 2);
    uint32 y = Console_y * CHARACTER_HEIGHT;

    for (size_t row = 0; row < CHARACTER_HEIGHT; row++) {
        uint8 characterRow = ConsoleFont[character][row];
        uint32 linearOffset = (y + row) * (Framebuffer.pitch >> 2);

        screen[(x + 0) + linearOffset] =
            (background ^ (-((characterRow >> 7) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 6) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);

        screen[(x + 1) + linearOffset] =
            (background ^ (-((characterRow >> 5) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 4) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);

        screen[(x + 2) + linearOffset] =
            (background ^ (-((characterRow >> 3) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 2) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);

        screen[(x + 3) + linearOffset] =
            (background ^ (-((characterRow >> 1) & 1U ) & (foreground ^ background))) |
            ((background ^ (-((characterRow >> 0) & 1U ) & (foreground ^ background))) << CHARACTER_HEIGHT);
    }
}

// ---------------------------------------------- //

/*
-> this function manages the formatting and how the characters are written on the screen;
-> contains newline detection and coordinate calculation, stuff like that;
-> it must keep checking if the characters might be out of bounds, so performance might drop;
*/
void OPTIMIZE(3) consoleWriteChar(enum consoleColours foreground, enum consoleColours background, char letter) {
    uint16 Foreground = ConsolePalette[foreground];
    uint16 Background = ConsolePalette[background];

    if (letter < 32 || Console.cursorY >= Console.rows) return;

    if (Console.cursorX >= Console.columns) {
        Console.cursorX = 0;
        Console.cursorY++;
    }

    renderCharacter(Foreground, Background, letter, Console.cursorX, Console.cursorY);
    Console.cursorX++;
}

void OPTIMIZE(3) consoleWriteCharXY(enum consoleColours foreground, enum consoleColours background, char letter, uint16 x, uint16 y) {
    uint16 Foreground = ConsolePalette[foreground];
    uint16 Background = ConsolePalette[background];

    if (letter < 32 || y >= Console.rows) return;

    if (x >= Console.columns) {
        x = 0;
        y++;
    }

    renderCharacter(Foreground, Background, letter, x, y);
}

void OPTIMIZE(3) consoleWrite(enum consoleColours foreground, enum consoleColours background, string text) {
    for (size_t index = 0; index < text.length; index++) {
        consoleWriteChar(foreground, background, text.text[index]);
    }
}

void OPTIMIZE(3) consoleWriteXY(enum consoleColours foreground, enum consoleColours background, string text, uint16 x, uint16 y) {
    for (size_t index = 0; index < text.length; index++) {
        consoleWriteCharXY(foreground, background, text.text[index], x, y);
        x++;
    }
}
// ------------------------------------------------ //
