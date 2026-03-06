#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <framebuffer.h>
#include <mailbox.h>

struct framebuffer_table mailbox_framebuffer = {
    .framebuffer_Init = mailboxFramebufferInit
};

struct gpio_table bcm_gpio_table = {
    .setFunction = bcm2835gpio_setFunction,
    .pinWrite = bcm2835gpio_pinWrite
};

struct gpio_table *gpio = &bcm_gpio_table;
struct framebuffer_table *framebuffer = &mailbox_framebuffer;
struct framebuffer_metadata framebuffer_metadata;

void fill_screen_blue(struct framebuffer_metadata *framebuffer) {
    if (!framebuffer || !framebuffer->pointer) return;

    uint32_t *pixel = (uint32_t *)framebuffer->pointer;
    uint32_t color = framebuffer->pixel_order ? 0xFF0000 : 0x0000FF; // BGR : RGB
    uint32_t bytes_per_pixel = framebuffer->depth / 8;

    for (uint16_t y = 0; y < framebuffer->virtual_height; y++) {
        for (uint16_t x = 0; x < framebuffer->virtual_width; x++) {
            *(uint32_t *)((uint8_t *)pixel + y * framebuffer->pitch + x * bytes_per_pixel) = color;
        }
    }
}

// Still Test code;
void kernel_main() {
    framebuffer_metadata.physical_width = 1024;
    framebuffer_metadata.physical_height = 768;
    framebuffer_metadata.virtual_width = 1024;
    framebuffer_metadata.virtual_height = 768;
    framebuffer_metadata.depth = 16;
    framebuffer_metadata.virtual_X_offset = 0;
    framebuffer_metadata.virtual_Y_offset = 0;

    gpio_setFunction(ACT_LED, OUTPUT);
    gpio_setFunction(PWR_LED, OUTPUT);
    bool status = framebufferInit(&framebuffer_metadata);
    if (status == 1) {
        gpio_pinWrite(ACT_LED, HIGH);
    }
}
