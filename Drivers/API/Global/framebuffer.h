#include <types.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/*
-> very important global structure;
-> pointer is just an address. In order to draw, you must cast it (i recomend casting to 32bit);
-> the system assumes that the depth will always be 16 (2 bytes per pixel);
-> pixelOrder == 0: RGB; pixelOrder == 1: BGR; 
*/

struct framebufferInfo {
    uintptr pointer;
    uint32 size;
    uint16 virtualHeight;
    uint16 virtualWidth;
    uint16 physicalHeight;
    uint16 physicalWidth;
    uint16 virtual_X_Offset;
    uint16 virtual_Y_Offset;
    uint16 pitch;
    uint16 depth;
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
    GlobalFramebufferTable.framebufferInit();
}

#endif
