#include <stdint.h>
#include <framebuffer.h>

#ifndef BITMAP_H
#define BITMAP_H

extern const uint8_t RedFont[127][16];
void drawCharacter(struct framebufferMetadata *screen, uint16_t colour, uint8_t character);

#endif
