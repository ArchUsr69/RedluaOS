#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <framebuffer.h>
#include <uart.h>
#include <console.h>

void prompt();
void consoleMain();

void prompt() {
    string prompt = stringNew(">", 0);
    consoleWrite(Red, Background, prompt);
}

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);

    consoleMain();
}

void consoleMain() {
    prompt();
    
    while(true) {
        char input[2];
        input[0] = uartReadByte();
        input[1] = '\0';

        if (input[0] == '\r') {

            GlobalConsole.cursorX = 0;
            GlobalConsole.cursorY++;

            prompt();
        }

        string text = stringNew(input, 0);

        consoleWrite(Foreground, Background, text);
    }
}
