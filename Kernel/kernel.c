#include <types.h>
#include <utils.h>
#include <string.h>
#include <gpio.h>
#include <uart.h>
#include <console.h>

void consoleMain();

void kernelMain() {
    gpioSetFunction(47, OUTPUT);
    gpioSetFunction(35, OUTPUT);
    gpioPinWrite(35, LOW);
    gpioPinWrite(47, HIGH);

    consoleMain();
}

void consoleMain() {
    string prompt = stringNew("[RedluaOS] > ", 0);
    string backspace = stringNew(" ", 0);
    
    consoleWrite(Red, Background, prompt);
    
    while(true) {
        if (Console.cursorY >= Console.rows) {

            Console.cursorY = 0;
            Console.cursorX = 0;
            
            for (size_t i = 0; i < (Console.rows * Console.columns); i++ ) {
                consoleWrite(Background, Background, backspace);
            }

            Console.cursorY = 0;
            Console.cursorX = 0;

            consoleWrite(Red, Background, prompt);
        }
        
        char input[2];
        input[0] = uartReadByte();
        input[1] = '\0';

        if (input[0] == '\r') {

            Console.cursorX = 0;
            Console.cursorY++;

            consoleWrite(Red, Background, prompt);

        } else if (input[0] == 0x8) {

            if (Console.cursorX <= prompt.length) Console.cursorX++;
            Console.cursorX--;
            consoleWrite(Background, Background, backspace);
            Console.cursorX--;

        } else {

        string text = stringNew(input, 0);
        consoleWrite(Foreground, Background, text);

        }
    }
}
