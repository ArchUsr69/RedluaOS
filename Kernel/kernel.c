#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>

// Fills the screen with F; Just try it out;
void kernelMain() {
    string prompt = newString("> ", 0);
    string message1 = newString("Welcome to RedluaOS", 0);
    string message2 = newString("This is fun", 0);

    consoleWrite(Red, Background, &prompt);
    consoleWrite(Foreground, Background, &message2);

    GlobalConsole.cursorY++;
    GlobalConsole.cursorX = 0;

    consoleWrite(Red, Background, &prompt); 
    consoleWrite(Foreground, Background, &message1);
}
