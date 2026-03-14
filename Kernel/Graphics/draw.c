#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <framebuffer.h>
#include <draw.h>

// Draw a single pixel at absolute coordinates
void drawPixel(struct framebufferMetadata *pixel, uint16_t x, uint16_t y, uint16_t colour) {
    if (x >= pixel->virtual_Width || y >= pixel->virtual_Height) return; // bounds check
    uint16_t *pointer = (uint16_t *)pixel->pointer;
    pointer[(y * pixel->pitch >> 1) + x] = colour;
}

// Draw a single character (8x16) at the current screen x/y
void drawCharacter(struct framebufferMetadata *screen, uint16_t colour, uint16_t character) {
    uint16_t startX = screen->x; // store starting coordinates
    uint16_t startY = screen->y;

    for (uint8_t row = 0; row < 16; row++) {
        uint8_t rowData = Unifont8x16[character][row]; // 8 bits for this row
        for (uint8_t col = 0; col < 8; col++) {
            // MSB = leftmost pixel, LSB = rightmost
            if ((rowData >> (7 - col)) & 1) {
                drawPixel(screen, startX + col, startY + row, colour);
            }
        }
    }

    // Move cursor to the right of this character
    screen->x += 8;
}
