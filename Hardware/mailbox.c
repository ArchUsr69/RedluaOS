#include <stdint.h>
#include <stdbool.h>
#include <mailbox.h>
#include <lowLevel.h>
#include <framebuffer.h>

/*
-> sends a pointer to the Buffer to the MAILBOX_WRITE register;
-> the pointer *MUST* be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> the function must wait until the register is ready to be written to;
-> Buffer size must be calculated so that the cache can be flushed accordingly;

What the register expects:

bit [31-4] -> pointer
bit [3-0] -> channel
*/

void mailboxWrite(uintptr_t pointer, uint8_t channel, uint16_t buffer_size) {
    if (channel > 9) return;
    while (*MAILBOX_READ_STATUS & MAILBOX_FULL) { /* spins */ }
    uint32_t register_contents = 0;
    register_contents = (uint32_t)pointer | channel;
    flush_dcache(pointer, buffer_size);
    *MAILBOX_WRITE = register_contents;
}

//------------------------------//

/*
-> reads from the MAILBOX_READ register;
-> Function only returns only the upper 28 bits from the register downshifted, so the upper 4 bits from the returned data will always be empty
*/

uint32_t mailboxRead(uint8_t channel) {
    if (channel > 9) return 0xFFFFFFFF; // means Failure
    uint32_t register_contents;
    while (true) {
        while (*MAILBOX_READ_STATUS & MAILBOX_EMPTY) { /* spins */ }
        register_contents = *MAILBOX_READ;
        if (readField32(&register_contents, 0, 4) == channel) {
            return readField32(&register_contents, 4, 28);
        }
    }
}

//--------------------------//


// Framebuffer message
static volatile uint32_t __attribute__((aligned(16))) Buffer[10];

/*
-> initializes a framebuffer;
-> takes a framebuffer_metadata struct pointer where it writes all the information about the framebuffer;
-> some values must already be initialized: see the values bellow;
*/

void mailboxFramebufferInit(struct framebuffer_metadata *framebuffer_metadata) {
    if (framebuffer_metadata->is_initialized) return;

    Buffer[0] = framebuffer_metadata->physical_width;
    Buffer[1] = framebuffer_metadata->physical_height;
    Buffer[2] = framebuffer_metadata->virtual_width;
    Buffer[3] = framebuffer_metadata->virtual_height;
    Buffer[4] = 0; // pitch
    Buffer[5] = framebuffer_metadata->depth;
    Buffer[6] = framebuffer_metadata->virtual_X_offset;
    Buffer[7] = framebuffer_metadata->virtual_Y_offset;
    Buffer[8] = 0; // framebuffer address (set by VC)
    Buffer[9] = 0; // framebuffer size (set by VC)

    // Send GPU bus address to mailbox
    mailboxWrite((uintptr_t)&Buffer | 0x40000000, 1, sizeof(Buffer));
    mailboxRead(1);

    // fills the requested information
    framebuffer_metadata->pitch = (uint16_t)Buffer[4];
    framebuffer_metadata->pointer = (uint8_t *)(Buffer[8] & 0x3FFFFFFF);
    framebuffer_metadata->size = Buffer[9];
    framebuffer_metadata->is_initialized = true;
}
