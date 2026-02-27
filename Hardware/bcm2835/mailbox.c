#include <stdint.h>
#include <stdbool.h>
#include "bcm2835.h"
#include "../../Libraries/lowLevel.h"

/*
-> VideoCore only returns and accepts bus addresses;
-> ARM only knows about physical addresses;
-> well, now you can see why you need these functions;

*IMPORTANT*:
exception is for property tags. In this case, VideoCore suddenly speaks in physical addresses only;
*/

inline uint32_t *physicalToBus(uint32_t *address, bool l2Cache_enabled) {
    uintptr_t translatedAddress = (uintptr_t)address;
    translatedAddress += (l2Cache_enabled ? 0xC0000000 : 0x40000000);
    return (uint32_t*)translatedAddress;
}

inline uint32_t *busToPhysical(uint32_t *address, bool l2Cache_enabled) {
    uintptr_t translatedAddress = (uintptr_t)address;
    translatedAddress -= (l2Cache_enabled ? 0xC0000000 : 0x40000000);
    return (uint32_t*)translatedAddress;
}

//------------------------------//

/*
-> writes data to the MAILBOX_WRITE register;
-> the data is just a pointer to a Buffer that contains the expected info for VideoCore;
-> the pointer must be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> data must be already aligned to upper 28 bits before passed into the function;
-> the function must wait until the register is ready to be written to;

What the register expects:

bit [31-4] -> pointer
bit [4-0] -> channel
*/

void mailboxWrite(uint8_t channel, uint32_t data) {
    while (*MAILBOX_STATUS & MAILBOX_FULL) { /* spins */ }
    uint32_t register_contents = 0;
    writeField32(&register_contents, data, 4, 28);
    writeField32(&register_contents, channel, 0, 4);
    *MAILBOX_WRITE = register_contents;
}

//------------------------------//

/*
-> reads from the MAILBOX_READ register;
-> if the channel you expect and the returned channel don't match, the function must repeat the process
-> Function only returns only the upper 28 bits from the register downshifted, so the upper 4 bits from the returned data will always be empty
*/

uint32_t mailboxRead(uint8_t channel) {
    uint32_t register_contents;
    do {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY) { /* spins */ }
        register_contents = readField32(MAILBOX_READ, 4, 28);
    } while (readField32(MAILBOX_READ, 0, 4) != channel);
    return register_contents;
}

//------------------------------//

/*
-> initializes a framebuffer;
-> it prepares the message buffer so that VC can respond to the tags this function sent;
-> VC allocates the framebuffer for you, and you can look at the data inside the message buffer
so you can use the framebuffer;
*/

void mailboxFramebufferInit() {
    struct mailboxTag allocate_buffer {
        .tag_id = FRAMEBUFFER_ALLOCATE;
        .size = 8;
        .request = 0;
        .data = {16, 0};
    }

    struct mailboxTag set_physical_size {
        .tag_id = FRAMEBUFFER_SET_PHYSICAL_SIZE;
        .size = 8;
        .request = 0;
        .data = {1920, 1080};
    }

    struct mailboxTag set_virtual_size {
        .tag_id = FRAMEBUFFER_SET_VIRTUAL_SIZE;
        .size = 8;
        .request = 0;
        .data = {1920, 1080};
    }

    struct mailboxTag set_depth {
        .tag_id = SET_DEPTH;
        .size = 4;
        .request = 0;
        .data = {8};
    }

    struct mailboxTag get_pitch {
        .tag_id = GET_PITCH;
        .size = 4;
        .request = 0;
        .data = {};
    }

    struct mailboxTag set_virtual_offset {
        .tag_id = SET_VIRTUAL_OFFSET;
        .size = 8;
        .request = 0;
        .data = {0, 0};
    }

    struct mailboxBuffer Buffer {
        .size = 8;
        .request = 0;
        .tags[0] = {16, 0};
    }

    mailboxWrite(8, physicalToBus(&Buffer, L2_cache_status));
}

//----------------------------//
