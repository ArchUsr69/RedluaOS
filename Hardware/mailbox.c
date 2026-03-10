#include <stdint.h>
#include <stdbool.h>
#include <mailbox.h>
#include <lowLevel.h>
#include <framebuffer.h>

/*
-> sends a pointer of the message buffer to VideoCore;
-> the pointer *MUST* be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> the function must wait until the register is ready to be written to;
-> Buffer size must be calculated so that the cache can be flushed accordingly;

What the register expects:

bit [31-4] -> pointer
bit [3-0] -> channel
*/

void mailboxWrite(uintptr_t pointer, enum mailboxChannels channel, size_t bufferSize) {
    if (channel == UNDEFINED) return;
    while (*MAILBOX_READ_STATUS & MAILBOX_FULL) { /* spins */ }
    flush_dcache(pointer, bufferSize);
    *MAILBOX_WRITE = pointer | channel;
}

//------------------------------//

/*
-> reads what VideoCore sent us back;
-> Function only returns only the upper 28 bits from the register downshifted;
-> must be called so that VC responds to the sent message;
*/

uint32_t mailboxRead(enum mailboxChannels channel) {
    if (channel == UNDEFINED) return 788; // idk, just a random number. I'll find a substitue later
    while (true) {
        while (*MAILBOX_READ_STATUS & MAILBOX_EMPTY) { /* spins */ }
        uint32_t registerContents = *MAILBOX_READ;
        if (registerContents & channel) {
            return readField32(&registerContents, 4, 28);
        }
    }
}

//--------------------------//

// Framebuffer message
static volatile uint32_t __attribute__((aligned(16))) Buffer[10];

/*
-> initializes a framebuffer;
-> takes a framebufferMetadata struct pointer where it writes all the information about the framebuffer;
-> some values must already be initialized: see the values bellow;
*/

void mailboxFramebufferInit(struct framebufferMetadata *framebufferMetadata) {
    if (framebufferMetadata->is_Initialized) return;

    Buffer[0] = framebufferMetadata->physical_Width;
    Buffer[1] = framebufferMetadata->physical_Height;
    Buffer[2] = framebufferMetadata->virtual_Width;
    Buffer[3] = framebufferMetadata->virtual_Height;
    Buffer[4] = 0; // pitch
    Buffer[5] = framebufferMetadata->depth;
    Buffer[6] = framebufferMetadata->virtual_X_Offset;
    Buffer[7] = framebufferMetadata->virtual_Y_Offset;
    Buffer[8] = 0; // framebuffer address (set by VC)
    Buffer[9] = 0; // framebuffer size (set by VC)

    // Send GPU bus address to mailbox (4 attempts)
    for (uint8_t attempts = 0; attempts < 4; attempts++) {
        mailboxWrite((uintptr_t)&Buffer | VC_OFFSET, FRAMEBUFFER, sizeof(Buffer));
        mailboxRead(FRAMEBUFFER);
        if (framebufferMetadata->pointer) { break; }
    }

    // fills the requested information
    framebufferMetadata->pitch = (uint16_t)Buffer[4];
    framebufferMetadata->pointer = (uint8_t *)((uintptr_t)Buffer[8] & ARM_OFFSET);
    framebufferMetadata->size = (size_t)Buffer[9];
    framebufferMetadata->is_Initialized = true;
}
