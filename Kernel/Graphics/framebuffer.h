#include <stdint.h>
#include <stdbool.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct framebufferMetadata {
    uint8_t *pointer;
    uint32_t size;
    uint16_t virtual_Height;
    uint16_t virtual_Width;
    uint16_t physical_Height;
    uint16_t physical_Width;
    uint16_t virtual_X_Offset;
    uint16_t virtual_Y_Offset;
    uint16_t depth;
    uint16_t pitch;
    bool pixel_Order;
    bool is_Initialized;
};

// Function table;
struct framebufferTable {
    void (*framebufferInit)(struct framebufferMetadata *framebufferMetadata);
};

// pointer to global struct instance; defined by the kernel
extern struct framebufferTable *framebuffer;

// Wrapper functions
static inline void framebufferInit(struct framebufferMetadata *framebufferMetadata) {
    return framebuffer->framebufferInit(framebufferMetadata);
}

#endif
