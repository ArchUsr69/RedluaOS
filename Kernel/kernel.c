#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <framebuffer.h>
#include <mailbox.h>

// Hardcoded driver selection
struct framebuffer_table mailbox_framebuffer = {
    .framebuffer_Init = mailboxFramebufferInit
};

struct gpio_table bcm_gpio_table = {
    .setFunction = bcm2835gpio_setFunction,
    .pinWrite = bcm2835gpio_pinWrite
};

// Global pointers definitions
struct gpio_table *gpio = &bcm_gpio_table;
struct framebuffer_table *framebuffer = &mailbox_framebuffer;

// Allocating a framebuffer metadata structure that holds information about the framebuffer 
struct framebuffer_metadata framebuffer_metadata;

// It creates a colour gradient that switches between 3 colours; looks very cool tbh;
void kernel_main() {
    framebuffer_metadata.physical_width = 1920;
    framebuffer_metadata.physical_height = 1080;
    framebuffer_metadata.virtual_width = 1920;
    framebuffer_metadata.virtual_height = 1080;
    framebuffer_metadata.depth = 16;
    framebuffer_metadata.virtual_X_offset = 0;
    framebuffer_metadata.virtual_Y_offset = 0;

    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_pinWrite(ACT_LED, HIGH);
    framebufferInit(&framebuffer_metadata);

    uint16_t *framebufferPointer = (uint16_t *)framebuffer_metadata.pointer;
    size_t pixelCount = framebuffer_metadata.size / 2;
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
