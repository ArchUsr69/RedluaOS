#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
// ------------------ //
#include <broadcom.h>
#include <utils.h>
#include <framebuffer.h>

// for now Hardcoded; It should be PERIPHERAL_BASE + MAILBOX_BASE offset
#define MAILBOX_BASE (PERIPHERAL_BASE + 0xB880)

// Mailbox 0 Registers (*NEVER WRITE TO THESE REGISTERS*)
#define MAILBOX_READ ((REGISTER_32 *)(MAILBOX_BASE + 0x00))
#define MAILBOX_READ_PEEK ((REGISTER_32 *)(MAILBOX_BASE + 0x10))
#define MAILBOX_READ_SENDER ((REGISTER_32 *)(MAILBOX_BASE + 0x14))
#define MAILBOX_READ_STATUS ((REGISTER_32 *)(MAILBOX_BASE + 0x18))
#define MAILBOX_READ_CONFIG ((REGISTER_32 *)(MAILBOX_BASE + 0x1C))

// Mailbox 1 Registers (You shouldn't read these registers; not that you have to)
#define MAILBOX_WRITE ((REGISTER_32 *)(MAILBOX_BASE + 0x20))
#define MAILBOX_WRITE_PEEK ((REGISTER_32 *)(MAILBOX_BASE + 0x30))
#define MAILBOX_WRITE_SENDER ((REGISTER_32 *)(MAILBOX_BASE + 0x34))
#define MAILBOX_WRITE_STATUS ((REGISTER_32 *)(MAILBOX_BASE + 0x38))
#define MAILBOX_WRITE_CONFIG ((REGISTER_32 *)(MAILBOX_BASE + 0x3C))

// Mailbox Status Flags
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

// Buffer status Flags
#define BUFFER_PARSE_SUCCESS 0x80000000
#define BUFFER_PARSE_FAILURE 0x80000001

/*
-> Returned when trying read from Channel 7; It is undefined / unused, so just don't use it;
-> unless you work for Broadcom and know what this channel does;
*/

#define UNDEFINED_CHANNEL_USAGE 0x80000002

// ------------------- //

/*
-> if ARM -> VC, bitwise OR the address (address | VC_OFFSET);
-> if VC -> ARM, bitwise AND the returned address (address & ARM_OFFSET);
-> Exception is when you use the property interface channel; then somehow VC speaks in physical addresses;
*/

#define VC_OFFSET 0x40000000
#define ARM_OFFSET 0x3FFFFFFF

// ------------------- //

enum mailboxChannels {
    POWER,
    FRAMEBUFFER,
    VIRTUAL_UART,
    VCHIQ,
    LED,
    BUTTONS,
    TOUCH_SCREEN,
    UNDEFINED, // don't use this channel
    PROPERTY_TAGS
};

// Unused for now
enum mailboxTags {
    FIRMWARE_REVISION = 0x00000001,
    FRAMEBUFFER_ALLOCATE = 0x00040001,
    FRAMEBUFFER_RELEASE = 0x00048001,
    SCREEN_BLANK = 0x00040002,
    GET_PHYSICAL_SIZE = 0x00040003,
    TEST_PHYSICAL_SIZE = 0x00044003,
    SET_PHYSICAL_SIZE = 0x00048003,
    GET_VIRTUAL_SIZE = 0x00040004,
    TEST_VIRTUAL_SIZE = 0x00044004,
    SET_VIRTUAL_SIZE = 0x00048004,
    GET_DEPTH = 0x00040005,
    TEST_DEPTH = 0x00044005,
    SET_DEPTH = 0x00048005,
    GET_PITCH = 0x00040008,
    GET_VIRTUAL_OFFSET = 0x00040009,
    TEST_VIRTUAL_OFFSET = 0x00044009,
    SET_VIRTUAL_OFFSET = 0x00048009,
    GET_PIXELORDER = 0x00040006,
    TEST_PIXELORDER = 0x00044006,
    SET_PIXELORDER = 0x00048006
};

/*
-> Property interface mailbox buffer structure (unused for now);
-> size must be calculated;
-> requestResponse must always be 0 initialized; VC will overwrite with 0x80000000 for success, 0x80000001 for fail;
-> Unused for now;
*/

struct mailboxBuffer {
    uint32_t size;
    uint32_t requestResponse;
    uint32_t tags[];
};

// ---------------------------- //

/*
-> sends a pointer of the message buffer to VideoCore;
-> the pointer *MUST* be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> the function must wait until the register is ready to be written to;
-> Buffer size must be calculated so that the cache can be flushed accordingly;

   What the register expects:

   bit [31-4] -> pointer;
   bit [3-0] -> channel;
*/

void mailboxWrite(uintptr_t pointer, enum mailboxChannels channel) {
    if (channel == UNDEFINED) return;
    while ((*MAILBOX_WRITE_STATUS & MAILBOX_FULL) != 0) { /* spins */ }
    *MAILBOX_WRITE = pointer | channel;
}

// ------------------------------ //

/*
-> reads what VideoCore sent us back;
-> Function only returns the upper 28 bits from the register downshifted;
-> must be called so that VC responds to the sent message;
-> will return 0x80000002 if trying to read channel 7;
*/

uint32_t mailboxRead(enum mailboxChannels channel) {
    if (channel == UNDEFINED) return UNDEFINED_CHANNEL_USAGE;
    while (true) {
        while ((*MAILBOX_READ_STATUS & MAILBOX_EMPTY) != 0) { /* Spins */ }
        uint32_t registerContents = *MAILBOX_READ;
        if (registerContents & channel == channel) return registerContents >> 4;
    }
}

// -------------------------- //

/*
-> takes a framebufferMetadata struct pointer where it writes all the information about the framebuffer;
-> All values must already be initialized; 
-> it is pretty messy, but it works pretty fine;
-> the order of the information can be found inside the array. *DO NOT CHANGE*
-> every word must have 32 bits;
*/

volatile uint32_t __attribute__((aligned(16))) messageBuffer[10];

void BCMframebufferInit() {;
    if (GlobalFramebuffer.pointer != 0) return;

    messageBuffer[0] = GlobalFramebuffer.physicalWidth;
    messageBuffer[1] = GlobalFramebuffer.physicalHeight;
    messageBuffer[2] = GlobalFramebuffer.virtualWidth;
    messageBuffer[3] = GlobalFramebuffer.virtualHeight;
    messageBuffer[4] = GlobalFramebuffer.pitch;
    messageBuffer[5] = GlobalFramebuffer.depth;
    messageBuffer[6] = GlobalFramebuffer.virtual_X_Offset;
    messageBuffer[7] = GlobalFramebuffer.virtual_Y_Offset;
    messageBuffer[8] = GlobalFramebuffer.pointer;
    messageBuffer[9] =  GlobalFramebuffer.size;

    mailboxWrite((uintptr_t)messageBuffer | VC_OFFSET, FRAMEBUFFER);
    mailboxRead(FRAMEBUFFER);
    
    // Fills whatever VC sent back;
    GlobalFramebuffer.physicalWidth = (uint16_t)messageBuffer[0];
    GlobalFramebuffer.physicalHeight = (uint16_t)messageBuffer[1];
    GlobalFramebuffer.pitch = (uint16_t)messageBuffer[4];
    GlobalFramebuffer.pointer = (uintptr_t)messageBuffer[8] & ARM_OFFSET;
    GlobalFramebuffer.size = (size_t)messageBuffer[9];
}

// -------------------------- //


/*
-> functions that manage Cache;
-> without those, the mailbox interface wouldn't probably work;
-> don't try to understand much. It's magic. Even i don't understand this crap;
-> They are for now Unused;
*/

static inline void flushCache(uintptr_t address, size_t size) {
    uintptr_t start = address & ~31;
    uintptr_t end = address + size;

    for (uintptr_t i = start; i < end; i += 32) {
        asm volatile (
            "mcr p15, 0, %0, c7, c14, 1\n"
            :
            : "r" (i)
            : "memory"
        );
    }
}

static inline void invalidateCache(uintptr_t address, size_t size) {
    uintptr_t start = address & ~31;
    uintptr_t end = address + size;

    for (uintptr_t i = start; i < end; i += 32) {
        asm volatile (
            "mcr p15, 0, %0, c7, c6, 1\n"
            :
            : "r" (i)
            : "memory"
        );
    }
}

// ------------------------------ //
