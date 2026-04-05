#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <framebuffer.h>
#include <console.h>

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    if (GlobalFramebuffer.pointer = 0x80000001) gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);

    string prompt = stringNew("> ", 0);
    string message1 = stringNew("Welcome to RedluaOS", 0);
    string message2 = stringNew("response from VC = ", 0);
    string value = stringNew(hex2string(GlobalFramebuffer.pointer), 0);

    consoleWrite(Red, Background, prompt);
    consoleWrite(Foreground, Background, message1);

    GlobalConsole.cursorY++;
    GlobalConsole.cursorX = 0;

    consoleWrite(Red, Background, prompt); 
    consoleWrite(Foreground, Background, message2);
    consoleWrite(Foreground, Background, value);
}
