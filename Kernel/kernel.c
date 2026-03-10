#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <framebuffer.h>
#include <mailbox.h>

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
    .depth = 16
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
    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_setFunction(PWR_LED, OUTPUT);
    gpio_pinWrite(PWR_LED, LOW);
    gpio_pinWrite(ACT_LED, LOW);
    framebufferInit(&framebufferMetadata);
    uint16_t *pointer = (uint16_t *)framebufferMetadata.pointer;

    for (size_t i = 0; i < (framebufferMetadata.size / 2); i++) {
        pointer[i] = OneDarker.blue;
    }
}
