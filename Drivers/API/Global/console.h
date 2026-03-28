#include <stdint.h>
#include <string.h>

#ifndef CONSOLE_H
#define CONSOLE_H

enum consoleColours {
    Background,
    Foreground,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BrightBlack,
    BrightRed,
    brightGreen,
    brightYellow,
    brightBlue,
    brightMagenta,
    brightCyan,
    brightWhite
};


/*
-> The main console struct;
-> just holds basic information that helps with text rendering;
*/

struct consoleInfo {
    uint16_t rows;
    uint16_t columns;
    uint16_t cursorX;
    uint16_t cursorY;
};

extern struct consoleInfo GlobalConsole;

// ------------------ //

void consoleInit();
void consoleWriteCharacter(enum consoleColours foreground, enum consoleColours background, char character);
void consoleWriteText(enum consoleColours foreground, enum consoleColours background, char *text, size_t length);

#endif
