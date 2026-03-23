#include <stdint.h>
#include <stdbool.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct framebufferMetadata {
    struct colourPalette *colourPalette;
    uintptr_t pointer;
    uintptr_t virtual_pointer;
    uint32_t size;
    uint16_t virtual_Height;
    uint16_t virtual_Width;
    uint16_t physical_Height;
    uint16_t physical_Width;
    uint16_t virtual_X_Offset;
    uint16_t virtual_Y_Offset;
    uint16_t pitch;
    uint16_t depth;
    uint16_t x;
    uint16_t y;
    bool pixel_Order;
    bool is_Initialized;
};

struct colourPalette {
    uint16_t background;
    uint16_t foreground;
    uint16_t black;
    uint16_t red;
    uint16_t green;
    uint16_t yellow;
    uint16_t blue;
    uint16_t magenta;
    uint16_t cyan;
    uint16_t white;
    uint16_t brightBlack;
    uint16_t brightRed;
    uint16_t brightGreen;
    uint16_t brightYellow;
    uint16_t brightBlue;
    uint16_t brightMagenta;
    uint16_t brightCyan;
    uint16_t brightWhite;
};

// Function table;
struct framebufferTable {
    void (*framebufferInit)(struct framebufferMetadata *framebufferMetadata);
};

// pointer to global struct instance; defined by the kernel
extern struct framebufferTable *framebuffer;

// Wrapper functions
static inline void framebufferInit(struct framebufferMetadata *framebufferMetadata) {
    framebuffer->framebufferInit(framebufferMetadata);
}

#endif
