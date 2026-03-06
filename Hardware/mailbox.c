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
-> Buffer size must be calculated so that the cache can be flushed accordingly;

What the register expects:

bit [31-4] -> pointer
bit [3-0] -> channel
*/

void mailboxWrite(uint32_t *pointer, uint8_t channel, uint16_t buffer_size) {
    while (*MAILBOX_READ_STATUS & MAILBOX_FULL) { /* spins */ }
    uint32_t register_contents = 0;
    writeField32(&register_contents, (uint32_t)pointer, 4, 28);
    writeField32(&register_contents, channel, 0, 4);
    flush_dcache(pointer, buffer_size);
    *MAILBOX_WRITE = register_contents;
}

//------------------------------//

/*
-> reads from the MAILBOX_READ register;
-> Function only returns only the upper 28 bits from the register downshifted, so the upper 4 bits from the returned data will always be empty
*/

uint32_t mailboxRead(uint8_t channel) {
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

volatile uint32_t __attribute__((aligned(4096))) Buffer[10];

/*
-> initializes a framebuffer;
-> takes a framebuffer_metadata struct pointer where it writes all the information about the framebuffer;
-> some values must already be initialized: see the values bellow;
*/

bool mailboxFramebufferInit(struct framebuffer_metadata *framebuffer_metadata) {
    if (framebuffer_metadata->is_initialized) return 1;

    Buffer[0] = 1024;
    Buffer[1] = 768;
    Buffer[2] = 1024;
    Buffer[3] = 768;
    Buffer[4] = 0; // pitch
    Buffer[5] = 16;
    Buffer[6] = 0;
    Buffer[7] = 0;
    Buffer[8] = 0; // framebuffer address (set by VC)
    Buffer[9] = 0; // framebuffer size (set by VC)

    // Send GPU bus address to mailbox
    uintptr_t address = (uintptr_t)&Buffer | 0x40000000;
    address = address >> 4;
    mailboxWrite((uint32_t *)address, 1, sizeof(Buffer));
    mailboxRead(1);
    if (Buffer[8] == 0) { return 1; }

    framebuffer_metadata->pitch = (uint16_t)Buffer[4];
    framebuffer_metadata->pointer = (uint8_t *)(Buffer[8] & 0x3FFFFFFF);
    framebuffer_metadata->size = Buffer[9];
    framebuffer_metadata->is_initialized = true;
    return 0;
}
