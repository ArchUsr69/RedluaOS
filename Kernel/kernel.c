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
    .virtual_Y_Offset = 0
};

struct colourPalette OneDarker = {
    .background = 0x0000,
    .foreground = 0xADD7,
    .black = 0x0000,
    .red = 0xF13B,
    .green = 0x94E5,
    .yellow = 0xD38A,
    .blue = 0x4AAF,
    .magenta = 0xC31B,
    .cyan = 0x21D8,
    .white = 0xADD7,
    .brightBlack = 0x5366,
    .brightRed = 0xFF8E,
    .brightGreen = 0xAFE9,
    .brightYellow = 0xF18B,
    .brightBlue = 0x57FF,
    .brightMagenta = 0xDE5F,
    .brightCyan = 0x4DF0,
    .brightWhite = 0xFFFF
};

// Global pointers definitions for function tables
struct gpioTable *gpio = &bcmGpioTable;
struct framebufferTable *framebuffer = &mailboxFramebuffer;

// It creates a colour gradient that switches between the entire colour palette;
void kernel_main() {
    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_setFunction(PWR_LED, OUTPUT);
    gpio_pinWrite(PWR_LED, LOW)
    gpio_pinWrite(ACT_LED, LOW);
    framebufferInit(&framebufferMetadata);

    uint16_t *framebufferPointer = (uint16_t *)framebufferMetadata.pointer;
    size_t pixelCount = framebufferMetadata.size / 2;
    uint16_t *colour = &OneDarker->background;
    uint16_t *end = &OneDarker + 18;
    while (1) {
        if (colour < end) {colour = &OneDarker->background}
        for (size_t i = 0; i < pixelCount; i++) {
            framebufferPointer[i] = *colour;
        }
        colour++;
        }
    }
}
