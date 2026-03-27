#include <stdint.h>
#include <stdbool.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/*
-> very important global structure;
-> pointer is just an address. In order to draw, you must cast it (i recomend casting to 32bit);
-> the system assumes that the depth will always be 16 (2 bytes per pixel);
-> pixelOrder == 0: RGB; pixelOrder == 1: BGR; 
*/

struct framebufferInfo {
    uintptr_t pointer;
    uint32_t size;
    uint16_t virtualHeight;
    uint16_t virtualWidth;
    uint16_t physicalHeight;
    uint16_t physicalWidth;
    uint16_t virtual_X_Offset;
    uint16_t virtual_Y_Offset;
    uint16_t pitch;
    uint16_t depth;
    bool pixelOrder;
};

extern struct framebufferInfo GlobalFramebuffer;

// --------------------------------- //

// Driver table
struct framebufferTable {
    void (*framebufferInit)();
};

extern struct framebufferTable GlobalFramebufferTable;

// Wrapper functions
static inline void framebufferInit() {
    framebuffer.framebufferInit();
}

#endif
