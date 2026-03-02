#include <stdint.h>
#include <stdbool.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct framebuffer_metadata {
    uint32_t *pointer;
    uint16_t virtual_height;
    uint16_t virtual_width;
    uint16_t physical_height;
    uint16_t physical_width;
    uint16_t virtual_offset;
    uint8_t depth;
    uint8_t pitch;
};

// Function table;
struct framebuffer_table {
    void (*framebuffInit)(struct framebuffer_metadata *pointer);
};

extern struct framebuffer_table *framebuffer;

// Wrapper functions
static inline void framebufferInit(struct framebuffer_metadata *pointer) {
    framebuffer->framebuffInit(pointer);
}

#endif
