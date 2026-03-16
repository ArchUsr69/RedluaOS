#include <stdint.h>
// ---------------- //
#include <framebuffer.h>
#include <bitmap.h>
#include <text.h>

static void drawPixel(struct framebufferMetadata *pixel, uint16_t colour) {
    if (pixel->x >= pixel->virtual_Width || pixel->y >= pixel->virtual_Height) return;
    uint16_t *pointer = (uint16_t *)pixel->pointer;
    pointer[(pixel->y * (pixel->pitch >> 1)) + pixel->x] = colour;
}

// Draws a standard Character (very slow right now; i need a DMA Driver)
void drawCharacter(struct framebufferMetadata *screen, uint16_t colour, uint16_t character) {
    if (screen->x >= screen->virtual_Width) {
        screen->x = 0;
        screen->y += 16;
    }

    if (screen->y >= screen->virtual_Height) return;

    uint16_t startX = screen->x;
    uint16_t startY = screen->y;

    for (uint8_t row = 0; row < 16; row++) {
        screen->x = startX;
        screen->y = startY + row;

        for (uint8_t column = 0; column < 8; column++) {
            if (RedFont[character][row] & (0x80 >> column)) {
                drawPixel(screen, colour);
            }
            screen->x++;
        }
    }

    screen->x = startX + 8;
    screen->y = startY;
}
