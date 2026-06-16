#include <Kernel/framebuffer.h>
#include <Kernel/console.h>
#include <Drivers/broadcom.h>

struct console Console = {0};

void redConsole();
void new_line();

void kernelMain(){
    Framebuffer.init();
    consoleInit();

    consoleWrite(Foreground, Background, "Welcome to RedluaOS");
    new_line();
    consoleWrite(Foreground, Background, "Resolution is 1080p");
    new_line();
    consoleWrite(Foreground, Background, "Source code: https://github.com/ArchUsr69/RedluaOS");
    new_line();
    new_line();
    redConsole();
}
