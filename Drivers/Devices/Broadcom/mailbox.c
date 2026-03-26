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

enum mailboxTags {
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
-> only allocates 32 Words for tags; Try not to use so many tags at once;
*/

struct mailboxBuffer {
    uint32_t size;
    uint32_t requestResponse;
    uint32_t tags[32];
};

// ---------------------------- //

/*
-> functions that manage Cache;
-> without those, the mailbox interface wouldn't probably work;
-> don't try to understand much. It's magic. Even i don't understand this crap;
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

// (unused for now);
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

static void mailboxWrite(uintptr_t pointer, enum mailboxChannels channel, size_t bufferSize) {
    if (channel == UNDEFINED) return;
    while (*MAILBOX_READ_STATUS & MAILBOX_FULL != 0) { /* spins */ }
    flushCache(pointer, bufferSize);
    *MAILBOX_WRITE = pointer | channel;
}

// ------------------------------ //

/*
-> reads what VideoCore sent us back;
-> Function only returns the upper 28 bits from the register downshifted;
-> must be called so that VC responds to the sent message;
-> will return 0x80000002 if trying to read channel 7;
*/

static uint32_t mailboxRead(enum mailboxChannels channel) {
    if (channel == UNDEFINED) return UNDEFINED_CHANNEL_USAGE;
    while (true) {
        while (*MAILBOX_READ_STATUS & MAILBOX_EMPTY != 0) { /* spins */ }
        uint32_t registerContents = *MAILBOX_READ;
        if (registerContents & channel != 0) {
            return registerContents >> 4;
        }
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

void BCMFramebufferInit() {;
    if (screen->pointer != 0) return;

    // The Ugly array that holds the information;
    volatile uint32_t __attribute__((aligned(16))) messageBuffer[10] = {
        framebuffer.physical_Width,
        framebuffer.physical_Height,
        framebuffer.virtual_Width,
        framebuffer.virtual_Height,
        framebuffer.pitch,            // *SET TO 0*
        framebuffer.depth,
        framebuffer.virtual_X_Offset,
        framebuffer.virtual_Y_Offset,
        framebuffer.pointer,          // *SET TO 0*
        framebuffer.size             // *SET TO 0*
    };

    // Send the Message Buffer (4 attempts);
    for (uint8_t attempts = 0; attempts < 4; attempts++) {
        mailboxWrite((uintptr_t)&messageBuffer | VC_OFFSET, FRAMEBUFFER, sizeof(messageBuffer));
        mailboxRead(FRAMEBUFFER);
        if (legacyFramebuffer[8] != 0) { break; }
    }

    // Fills whatever VC sent back;
    framebuffer.physical_Width = (uint16_t)messageBuffer[0];
    framebuffer.physical_Height = (uint16_t)messageBuffer[1];
    framebuffer.pitch = (uint16_t)messageBuffer[4];
    framebuffer.pointer = (uintptr_t)messageBuffer[8] & ARM_OFFSET;
    framebuffer.size = (size_t)messageBuffer[9];

// -------------------------- //

}
