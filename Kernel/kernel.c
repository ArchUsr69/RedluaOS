#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>

// Fills the screen with F; Just try it out;
void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);

    uint8 A = 255;
    string prompt = stringNew("> ", 0);
    string message1 = stringNew("Welcome to RedluaOS", 0);
    string message2 = stringNew("Address of random variable A = ", 0);
    string message3 = stringNew("Value of A in Binary = ", 0);
    string value = stringNew(bin2string(A), 0);
    string address = stringNew(hex2string((uintptr)&A), 0);

    consoleWrite(Red, Background, &prompt);
    consoleWrite(Foreground, Background, &message1);

    GlobalConsole.cursorY++;
    GlobalConsole.cursorX = 0;

    consoleWrite(Red, Background, &prompt); 
    consoleWrite(Foreground, Background, &message2);
    consoleWrite(Foreground, Background, &address);

    GlobalConsole.cursorY++;
    GlobalConsole.cursorX = 0;

    consoleWrite(Red, Background, &prompt);
    consoleWrite(Foreground, Background, &message3);
    consoleWrite(Foreground, Background, &value);
}
