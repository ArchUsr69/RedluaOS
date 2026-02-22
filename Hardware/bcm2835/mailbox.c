#include <stdint.h>
#include <stdbool.h>
#include "bcm2835.h"
#include "../../Libraries/lowLevel.h"

// translates the address formats; VC uses bus addresses, ARM uses physical
uint32_t physicalToBus(uint32_t address, bool l2Cache_enabled) {
    if (l2Cache_enabled == 0) {
        return address + 0xC0000000;
    } else {
        return address + 0x40000000;
    }
}

uint32_t busToPhysical(uint32_t address, bool l2Cache_enabled) {
    if (l2Cache_enabled == 0) {
        return address - 0xC0000000;
    } else {
        return address - 0x40000000;
    }
}

//------------------//

void mailboxWrite(uint8_t channel, uint32_t data) {
    while(*MAILBOX_STATUS & MAILBOX_FULL) { /* spins, does nothing */ }
    bit32_setSection(MAILBOX_WRITE, data, 4, 28);
    bit32_setSection(MAILBOX_WRITE, channel, 0, 4);
}

