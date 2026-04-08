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

uint16 consolePalette[18] = {
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
    if (GlobalFramebuffer.pointer == 0 || GlobalConsole.rows != 0) return;

    GlobalConsole.rows = GlobalFramebuffer.virtualHeight >> (CHARACTER_HEIGHT >> 2);
    GlobalConsole.columns = GlobalFramebuffer.virtualWidth >> 3;
    GlobalConsole.cursorX = 0;
    GlobalConsole.cursorY = 0;
};

// ---------------------- //

/*
-> a mathematical monster basically;
-> It is pretty fragile, so don't try to change much from the math;
-> It assumes depth = 16, so make sure that it is, or else it won't work;
-> It also assumes the Font is 8x16, in order to get more performance;
-> Writes 4 bytes at once to squeeze performance;
*/

void OPTIMIZE(3) consoleWriteCharacter(enum consoleColours foreground, enum consoleColours background, char character) {
    if (GlobalConsole.cursorX >= GlobalConsole.columns) {
        GlobalConsole.cursorX = 0;
        GlobalConsole.cursorY++;
    }

    if (GlobalConsole.cursorY >= GlobalConsole.rows) return;

    uint16 x = GlobalConsole.cursorX * (CHARACTER_WIDTH >> 1);
    uint16 y = GlobalConsole.cursorY * CHARACTER_HEIGHT;
    uint32 *screen = (uint32 *)GlobalFramebuffer.pointer;
    uint16 Foreground = consolePalette[foreground];
    uint16 Background = consolePalette[background];


    for (uint8 row = 0; row < CHARACTER_HEIGHT; row++) {
        uint8 characterRow = consoleFont[character][row];
        uint32 linearOffset = (y + row) * (GlobalFramebuffer.pitch >> 2);

        screen[(x + 0) + linearOffset] =
            (Background ^ (-( (characterRow >> 7) & 1U ) & Foreground)) |
            ((Background ^ (-( (characterRow >> 6) & 1U ) & Foreground)) << CHARACTER_HEIGHT);

        screen[(x + 1) + linearOffset] =
            (Background ^ (-( (characterRow >> 5) & 1U ) & Foreground)) |
            ((Background ^ (-( (characterRow >> 4) & 1U ) & Foreground)) << CHARACTER_HEIGHT);

        screen[(x + 2) + linearOffset] =
            (Background ^ (-( (characterRow >> 3) & 1U ) & Foreground)) |
            ((Background ^ (-( (characterRow >> 2) & 1U ) & Foreground)) << CHARACTER_HEIGHT);

        screen[(x + 3) + linearOffset] =
            (Background ^ (-( (characterRow >> 1) & 1U ) & Foreground)) |
            ((Background ^ (-( (characterRow >> 0) & 1U ) & Foreground)) << CHARACTER_HEIGHT);
    }
    GlobalConsole.cursorX++;
}

void OPTIMIZE(3) consoleWrite(enum consoleColours foreground, enum consoleColours background, string text) {
    for (size_t character = 0; character < text.length; character++) {
        consoleWriteCharacter(foreground, background, text.text[character]);      
        if (text.text[character] == '\n') {
            GlobalConsole.cursorX = 0;
            GlobalConsole.cursorY++;
        }
    }
}

// ------------------------------------------------ //
