#include <Kernel/framebuffer.h>
#include <Kernel/console.h>

void redConsole();

void newLine() {
    Console.cursorX = 0;
    Console.cursorY++;
}

void kernelMain(){
    Framebuffer.init();
    consoleInit();

    consoleWrite(Foreground, Background, "RedluaOS v0.5.12 Console");
    newLine();
    consoleWrite(Foreground, Background, "Resolution is 1080p");
    newLine();
    consoleWrite(Foreground, Background, "Source code: https://github.com/ArchUsr69/RedluaOS");
    newLine();
    redConsole();
}
