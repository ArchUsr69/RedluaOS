#include <stdint.h>
// ---------------- //
#include <framebuffer.h>

#ifndef TEXT_H
#define TEXT_H

void drawCharacter(struct framebufferMetadata *screen, uint16_t colour, uint16_t character);

#endif
