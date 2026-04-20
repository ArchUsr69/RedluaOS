#include <types.h>
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

struct console {
    uint16 rows;
    uint16 columns;
    uint16 cursorX;
    uint16 cursorY;
};

extern struct console Console;

// ------------------ //

void consoleInit();

void consoleWriteChar(enum consoleColours foreground, enum consoleColours background, char letter);
void consoleWriteCharXY(enum consoleColours foreground, enum consoleColours background, char letter, uint16 x, uint16 y);

void consoleWrite(enum consoleColours foreground, enum consoleColours background, string text);
void consoleWriteXY(enum consoleColours foreground, enum consoleColours background, string text, uint16 x, uint16 y);

#endif
