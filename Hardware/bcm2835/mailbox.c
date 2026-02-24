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

inline uint32_t physicalToBus(uint32_t address, bool l2Cache_enabled) {
    if (l2Cache_enabled) {
        return address + 0xC0000000;
    } else {
        return address + 0x40000000;
    }
}
inline uint32_t busToPhysical(uint32_t address, bool l2Cache_enabled) {
    if (l2Cache_enabled) {
        return address - 0xC0000000;
    } else {
        return address - 0x40000000;
    }
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
