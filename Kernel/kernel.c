#include <Kernel/framebuffer.h>
#include <Kernel/console.h>
#include <Drivers/broadcom.h>

struct framebuffer Framebuffer = {
    .init = BCMframebufferInit,
    .virtualHeight = 1080,
    .virtualWidth = 1920,
    .physicalHeight = 1080,
    .physicalWidth = 1920,
    .virtual_X_Offset = 0,
    .virtual_Y_Offset = 0,
    .depth = 16,
    .pixelOrder = 1
};

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
