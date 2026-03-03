#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <lowLevel.h>
#include <framebuffer.h>

/*
-> sends a pointer to the Buffer to the MAILBOX_WRITE register;
-> the pointer must be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> data must be already aligned to upper 28 bits before passed into the function;
-> the function must wait until the register is ready to be written to;

What the register expects:

bit [31-4] -> pointer
bit [4-0] -> channel 8 only (hardcoded)
*/

static void mailboxWrite(struct mailboxBuffer *pointer) {
    while (*MAILBOX_WRITE_STATUS & MAILBOX_FULL) { /* spins */ }
    uint32_t register_contents = 0;
    writeField32(&register_contents, (uint32_t)pointer, 4, 28);
    writeField32(&register_contents, MAILBOX_CHANNEL, 0, 4);
    flush_dcache(pointer, sizeof(pointer));
    *MAILBOX_WRITE = register_contents;
}

//------------------------------//

/*
-> reads from the MAILBOX_READ register;
-> Function only returns only the upper 28 bits from the register downshifted, so the upper 4 bits from the returned data will always be empty
*/

static uint32_t mailboxRead() {
    uint32_t register_contents;
    do {
        while (*MAILBOX_READ_STATUS & MAILBOX_EMPTY) { /* spins */ }
        register_contents = readField32(MAILBOX_READ, 4, 28);
    } while (readField32(MAILBOX_READ, 0, 4) != MAILBOX_CHANNEL);
    return register_contents;
}

//--------------------------//

/*
-> functions that manage Cache;
-> without those, the mailbox interface wouldn't even work, like, at all;
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
-> i know, it looks like a piece of shit, like cmon... manually manipulating values;
-> but the values get into that array anyway;
*/

bool mailboxFramebufferInit(struct framebuffer_metadata *framebuffer_metadata) {
    struct mailboxBuffer Buffer __attribute__((aligned(16)));

    Buffer.size = (2 + 37) * 4;
    Buffer.request_response = 0;

    // ---------------- //

    Buffer.tags[0] = FRAMEBUFFER_ALLOCATE;
    Buffer.tags[1] = 4;
    Buffer.tags[2] = 0;
    Buffer.tags[3] = 16;
    Buffer.tags[4] = 0;

    // ---------------- //

    Buffer.tags[5] = GET_PHYSICAL_SIZE;
    Buffer.tags[6] = 0;
    Buffer.tags[7] = 0;
    Buffer.tags[8] = 0;
    Buffer.tags[9] = 0;

    // ---------------- //

    Buffer.tags[10] = GET_VIRTUAL_SIZE;
    Buffer.tags[11] = 0;
    Buffer.tags[12] = 0;
    Buffer.tags[13] = 0;
    Buffer.tags[14] = 0;

    // ---------------- //

    Buffer.tags[15] = GET_DEPTH;
    Buffer.tags[16] = 0;
    Buffer.tags[17] = 0;
    Buffer.tags[18] = 0;

    // ---------------- //

    Buffer.tags[19] = GET_PITCH;
    Buffer.tags[20] = 0;
    Buffer.tags[21] = 0;
    Buffer.tags[22] = 0;

    // --------------- //

    Buffer.tags[23] = GET_PIXELORDER;
    Buffer.tags[24] = 0;
    Buffer.tags[25] = 0;
    Buffer.tags[26] = 0;

    // --------------- //

    Buffer.tags[27] = GET_VIRTUAL_OFFSET;
    Buffer.tags[28] = 0;
    Buffer.tags[29] = 0;
    Buffer.tags[30] = 0;
    Buffer.tags[31] = 0;

    // --------------- //

    Buffer.tags[32] = SCREEN_BLANK;
    Buffer.tags[33] = 4;
    Buffer.tags[34] = 0;
    Buffer.tags[35] = 1;

    // End Tag
    Buffer.tags[36] = 0;

    mailboxWrite(&Buffer);
    if (Buffer.request_response == BUFFER_PARSE_FAILURE || 0 || Buffer.tags[4] > 100) {
        return 1;
    }

    // fills metadata structure
    framebuffer_metadata->pointer = (uint8_t *)Buffer.tags[3];
    framebuffer_metadata->size = Buffer.tags[4];
    framebuffer_metadata->physical_width = (uint16_t)Buffer.tags[8];
    framebuffer_metadata->physical_height = (uint16_t)Buffer.tags[9];
    framebuffer_metadata->virtual_width = (uint16_t)Buffer.tags[13];
    framebuffer_metadata->virtual_height = (uint16_t)Buffer.tags[14];
    framebuffer_metadata->depth = (uint8_t)Buffer.tags[18];
    framebuffer_metadata->pitch = (uint8_t)Buffer.tags[22];
    framebuffer_metadata->pixel_order = (bool)Buffer.tags[26];
    framebuffer_metadata->virtual_X_offset = (uint16_t)Buffer.tags[30];
    framebuffer_metadata->virtual_Y_offset = (uint16_t)Buffer.tags[31];

    // ----------------- //

    Buffer.size = (2 + 24) * 4;
    Buffer.request_response = 0;

    Buffer.tags[0] = SET_PHYSICAL_SIZE;
    Buffer.tags[1] = 8;
    Buffer.tags[2] = 0;
    Buffer.tags[3] = framebuffer_metadata->physical_width;
    Buffer.tags[4] = framebuffer_metadata->physical_height;

    // ----------------- //

    Buffer.tags[5] = SET_VIRTUAL_SIZE;
    Buffer.tags[6] = 8;
    Buffer.tags[7] = 0;
    Buffer.tags[8] = framebuffer_metadata->virtual_width;
    Buffer.tags[9] = framebuffer_metadata->virtual_height;

    // ----------------- //

    Buffer.tags[10] = SET_DEPTH;
    Buffer.tags[11] = 4;
    Buffer.tags[12] = 0;
    Buffer.tags[13] = framebuffer_metadata->depth;

    // ---------------- //

    Buffer.tags[14] = SET_PIXELORDER;
    Buffer.tags[15] = 4;
    Buffer.tags[16] = 0;
    Buffer.tags[17] = framebuffer_metadata->pixel_order;

    // ---------------- //

    Buffer.tags[18] = SET_VIRTUAL_OFFSET;
    Buffer.tags[19] = 8;
    Buffer.tags[20] = 0;
    Buffer.tags[21] = framebuffer_metadata->virtual_X_offset;
    Buffer.tags[22] = framebuffer_metadata->virtual_Y_offset;

    // ---------------- //

    // End tag
    Buffer.tags[23] = 0;
    mailboxWrite(&Buffer);
    if (Buffer.request_response == BUFFER_PARSE_FAILURE || 0) {
        return 1;
    }
    return 0;
}

uint8_t test() {
    struct mailboxBuffer Buffer __attribute__((aligned(16)));

    Buffer.tags[0] = FRAMEBUFFER_ALLOCATE;
    Buffer.tags[1] = 4;
    Buffer.tags[2] = 0;
    Buffer.tags[3] = 16;
    Buffer.tags[4] = 0;

    // ---------------- //

    Buffer.tags[5] = SET_PHYSICAL_SIZE;
    Buffer.tags[6] = 8;
    Buffer.tags[7] = 0;
    Buffer.tags[8] = 1080;
    Buffer.tags[9] = 720;

    // ----------------- //

    Buffer.tags[10] = SET_VIRTUAL_SIZE;
    Buffer.tags[11] = 8;
    Buffer.tags[12] = 0;
    Buffer.tags[13] = 1080;
    Buffer.tags[14] = 720;

    // ----------------- //

    Buffer.tags[15] = SET_DEPTH;
    Buffer.tags[16] = 4;
    Buffer.tags[17] = 0;
    Buffer.tags[18] = 8;

    // ---------------- //

    Buffer.tags[19] = SET_PIXELORDER;
    Buffer.tags[20] = 4;
    Buffer.tags[21] = 0;
    Buffer.tags[22] = 0;

    // ---------------- //

    Buffer.tags[23] = SET_VIRTUAL_OFFSET;
    Buffer.tags[24] = 8;
    Buffer.tags[25] = 0;
    Buffer.tags[26] = 0;
    Buffer.tags[27] = 0;

    // ---------------- //

    Buffer.tags[28] = 0;

    Buffer.size = (2 + 29) * 4;
    Buffer.request_response = 0;

    mailboxWrite(&Buffer);
}
