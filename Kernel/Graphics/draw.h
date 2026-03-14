#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <framebuffer.h>

#ifndef DRAW_H
#define DRAW_H

void drawPixel(struct framebufferMetadata *pixel, uint16_t x, uint16_t y, uint16_t colour);
void drawCharacter(struct framebufferMetadata *screen, uint16_t colour, uint16_t character);

#endif
