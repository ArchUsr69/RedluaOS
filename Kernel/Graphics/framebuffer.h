#include <stdint.h>
#include <stdbool.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

struct framebuffer_metadata {
    uint8_t *pointer;
    uint32_t size;
    uint16_t virtual_height;
    uint16_t virtual_width;
    uint16_t physical_height;
    uint16_t physical_width;
    uint16_t virtual_X_offset;
    uint16_t virtual_Y_offset;
    uint8_t depth;
    uint8_t pitch;
    bool pixel_order;
    bool is_initialized;
};

// Function table;
struct framebuffer_table {
    void (*framebuffer_Init)(struct framebuffer_metadata *framebuffer_metadata);
};

// pointer to global struct instance
extern struct framebuffer_table *framebuffer;

// Wrapper functions
static inline void framebufferInit(struct framebuffer_metadata *framebuffer_metadata) {
    framebuffer->framebuffer_Init(framebuffer_metadata);
}

#endif
