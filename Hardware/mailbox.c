#include <stdint.h>
#include <stdbool.h>
#include <mailbox.h>
#include <lowLevel.h>
#include <framebuffer.h>

/*
-> sends a pointer to the Buffer to the MAILBOX_WRITE register;
-> the pointer must be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> pointer MUST be 16 byte aligned, or else it wouldn't fit;
-> the function must wait until the register is ready to be written to;

What the register expects:

bit [31-4] -> pointer
bit [4-0] -> channel
*/

static void mailboxWrite(uint32_t *pointer, uint8_t channel) {
    while (*MAILBOX_WRITE_STATUS & MAILBOX_FULL) { /* spins */ }
    uint32_t register_contents = 0;
    writeField32(&register_contents, (uint32_t)pointer, 4, 28);
    writeField32(&register_contents, channel, 0, 4);
    flush_dcache(pointer, sizeof(pointer));
    *MAILBOX_WRITE = register_contents;
}

//------------------------------//

/*
-> reads from the MAILBOX_READ register;
-> Function only returns only the upper 28 bits from the register downshifted, so the upper 4 bits from the returned data will always be empty
*/

static uint32_t mailboxRead(uint8_t channel) {
    uint32_t register_contents;
    do {
        while (*MAILBOX_READ_STATUS & MAILBOX_EMPTY) { /* spins */ }
        register_contents = readField32(MAILBOX_READ, 4, 28);
    } while (readField32(MAILBOX_READ, 0, 4) != channel);
    return register_contents;
}

//--------------------------//

/*
-> functions that manage Cache;
-> without those, the mailbox interface wouldn't even work;
-> don't try to understand much. It's magic. Even i don't understand this crap
*/

static inline void flush_dcache(void *address, uint8_t size) {
    uintptr_t start = (uintptr_t)address & ~31;
    uintptr_t end = (uintptr_t)address + size;

    for (uintptr_t i = start; i < end; i += 32) {
        asm volatile (
            "mcr p15, 0, %0, c7, c14, 1\n"
            :
            : "r" (i)
            : "memory"
        );
    }
}

static inline void invalidate_dcache(void *address, uint8_t size) {
    uintptr_t start = (uintptr_t)address & ~31;
    uintptr_t end = (uintptr_t)address + size;

    for (uintptr_t i = start; i < end; i += 32) {
        asm volatile (
            "mcr p15, 0, %0, c7, c6, 1\n"
            :
            : "r" (i)
            : "memory"
        );
    }
}

//------------------------------//

/*
-> initializes a framebuffer;
-> takes a framebuffer_metadata struct pointer where it writes all the information about the framebuffer; 
-> some values must already be initialized: see the values bellow;
*/

void mailboxFramebufferInit(struct framebuffer_metadata *framebuffer_metadata) {
    if (framebuffer_metadata->is_initialized = true) { return; } 

    uint32_t Buffer[10];

    // ---------------- //

    Buffer[0] = framebuffer_metadata->physical_width;
    Buffer[1] = framebuffer_metadata->physical_height;
    Buffer[2] = framebuffer_metadata->virtual_width;
    Buffer[3] = framebuffer_metadata->virtual_height;
    Buffer[4] = 0; // depth
    Buffer[5] = framebuffer_metadata->depth;
    Buffer[6] = framebuffer_metadata->virtual_X_offset;
    Buffer[7] = framebuffer_metadata->virtual_Y_offset;
    Buffer[8] = 0; // framebuffer address;
    Buffer[9] = 0; // framebuffer size;

    // ---------------- //

    mailboxWrite(&Buffer, 1);

    framebuffer_metadata->depth = Buffer[4];
    framebuffer_metadata->pointer = Buffer[8];
    framebuffer_metadata->size = Buffer[9];
 }