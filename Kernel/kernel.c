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

// Global pointers definitions
struct gpioTable *gpio = &bcmGpioTable;
struct framebufferTable *framebuffer = &mailboxFramebuffer;

// Allocating a framebuffer metadata structure that holds information about the framebuffer 
struct framebufferMetadata framebufferMetadata;

// It creates a colour gradient that switches between 3 colours; looks very cool tbh;
void kernel_main() {
    framebufferMetadata.physical_Width = 1920;
    framebufferMetadata.physical_Height = 1080;
    framebufferMetadata.virtual_Width = 1920;
    framebufferMetadata.virtual_Height = 1080;
    framebufferMetadata.depth = 16;
    framebufferMetadata.virtual_X_Offset = 0;
    framebufferMetadata.virtual_Y_Offset = 0;

    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_pinWrite(ACT_LED, LOW);
    framebufferInit(&framebufferMetadata);
    if (framebufferMetadata.virtual_Height != 1080) {
        gpio_pinWrite(ACT_LED, HIGH);
    }

    uint16_t *framebufferPointer = (uint16_t *)framebufferMetadata.pointer;
    size_t pixelCount = framebufferMetadata.size / 2;
    while (1) {
        for (size_t i = 0; i < pixelCount; i++) {
            framebufferPointer[i] = 0x1F; // Blue
        }
        for (size_t i = 0; i < pixelCount; i++) {
            framebufferPointer[i] = 0x07E0; // Green
        }
        for (size_t i = 0; i < pixelCount; i++) {
            framebufferPointer[i] = 0xF800; // Red
        }
    }
}
