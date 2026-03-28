#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ----------------- //
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>
#include <utils.h>

// Fills the screen with F; Just try it out;
void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioPinWrite(47, HIGH);

    consoleWriteText(Red, Background, "> ", 2);
    consoleWriteText(Foreground, Background, "Welcome to Redlua OS", 20);

    GlobalConsole.cursorY++;
    GlobalConsole.cursorX = 0;

    consoleWriteText(Red, Background, "> ", 2); 
    consoleWriteText(Foreground, Background, "This is fun", 11);
}
