#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ----------------- //
#include <gpio.h>
#include <bcm2835.h>
#include <framebuffer.h>
#include <mailbox.h>
#include <bitmap.h>
#include <utils.h>

// Hardcoded driver selection
struct framebufferTable mailboxFramebuffer = {
    .framebufferInit = mailboxFramebufferInit
};

struct gpioTable bcmGpioTable = {
    .setFunction = bcm2835gpio_setFunction,
    .pinWrite = bcm2835gpio_pinWrite
};

// Allocating a framebuffer metadata structure that holds information about the framebuffer
struct framebufferMetadata framebufferMetadata = {
    .physical_Width = 1920,
    .physical_Height = 1080,
    .virtual_Width = 1920,
    .virtual_Height = 1080,
    .virtual_X_Offset = 0,
    .virtual_Y_Offset = 0,
    .depth = 16,
    .x = 0,
    .y = 0
};

struct colourPalette OneDarker = {
    .background = 0x0000,
    .foreground = 0xFFFF,
    .black = 0x2966,
    .red = 0xDB6E,
    .green = 0x7EED,
    .yellow = 0xF7EC,
    .blue = 0x4D9E,
    .magenta = 0xFB3D,
    .cyan = 0x55B8,
    .white = 0xFFFF,
    .brightBlack = 0xAD97,
    .brightRed = 0xDB6E,
    .brightGreen = 0x7EED,
    .brightYellow = 0xF7EC,
    .brightBlue = 0x4D9E,
    .brightMagenta = 0xFB3D,
    .brightCyan = 0x55B8,
    .brightWhite = 0xFFFF
};

// Global pointers definitions for function tables
struct gpioTable *gpio = &bcmGpioTable;
struct framebufferTable *framebuffer = &mailboxFramebuffer;

// does practically nothing;
void kernel_main() {
    gpio_setFunction(STATUS_LED, OUTPUT);
    gpio_setFunction(POWER_LED, OUTPUT);
    gpio_pinWrite(POWER_LED, LOW);
    gpio_pinWrite(STATUS_LED, LOW);
    framebufferInit(&framebufferMetadata);

    char name[] = {"Hello, my name is Eduard. I hope this font looks good."};

    for (uint8_t i = 0; i < sizeof(name); i++) {
        drawCharacter(&framebufferMetadata, OneDarker.red, (uint8_t)name[i]);
    }
}

