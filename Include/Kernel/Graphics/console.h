#include <stdint.h>
#include <framebuffer.h>

#ifndef CONSOLE_H
#define CONSOLE_H

extern const uint8_t ConsoleFont[127][16];
void consoleWrite(struct framebufferMetadata *screen, uint16_t colour, uint8_t character);

#endif
