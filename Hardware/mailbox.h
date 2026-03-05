#include <stdint.h>
#include <stdbool.h>
#include <framebuffer.h>
#include <lowLevel.h>

#ifndef MAILBOX_H
#define MAILBOX_H

#define MAILBOX_BASE 0x2000B880

// Mailbox 0 Registers (*NEVER WRITE TO THESE REGISTERS*)
#define MAILBOX_READ ((volatile uint32_t *)(MAILBOX_BASE + 0x00))
#define MAILBOX_READ_PEEK ((volatile uint32_t *)(MAILBOX_BASE + 0x10))
#define MAILBOX_READ_SENDER ((volatile uint32_t *)(MAILBOX_BASE + 0x14))
#define MAILBOX_READ_STATUS ((volatile uint32_t *)(MAILBOX_BASE + 0x18))
#define MAILBOX_READ_CONFIG ((volatile uint32_t *)(MAILBOX_BASE + 0x1C))

// Mailbox 1 Registers (You shouldn't read these registers; not that you have to)
#define MAILBOX_WRITE ((volatile uint32_t *)(MAILBOX_BASE + 0x20))
#define MAILBOX_WRITE_PEEK ((volatile uint32_t *)(MAILBOX_BASE + 0x30))
#define MAILBOX_WRITE_SENDER ((volatile uint32_t *)(MAILBOX_BASE + 0x34))
#define MAILBOX_WRITE_STATUS ((volatile uint32_t *)(MAILBOX_BASE + 0x38))
#define MAILBOX_WRITE_CONFIG ((volatile uint32_t *)(MAILBOX_BASE + 0x3C))

// Mailbox Status Flags
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

// Buffer status Flags
#define BUFFER_PARSE_SUCCESS 0x80000000
#define BUFFER_PARSE_FAILURE 0x80000001

// Mailbox Tags
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
-> Property interface mailbox buffer structure;
-> size must be calculated;
-> request_response must always be 0 initialized; VC will overwrite with 0x80000000 for success, 0x80000001 for failure;
*/

struct mailboxBuffer {
    uint32_t size;
    uint32_t request_response;
    uint32_t tags[64];
};

// ---------------------------- //

// Function prototypes
void mailboxWrite(uint32_t *pointer, uint8_t channel, uint16_t buffer_size);
uint32_t mailboxRead(uint8_t channel);
void mailboxFramebufferInit(struct framebuffer_metadata *pointer);

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

#endif
