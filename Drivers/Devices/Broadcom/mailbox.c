#include <types.h>
#include <utils.h>
#include <broadcom.h>
#include <framebuffer.h>

#define MAILBOX_BASE (PERIPHERAL_BASE + 0xB880)

// Mailbox 0 Registers
#define MAILBOX_READ ((RO_MMIO_32)(MAILBOX_BASE + 0x00))
#define MAILBOX_READ_PEEK ((RO_MMIO_32)(MAILBOX_BASE + 0x10))
#define MAILBOX_READ_SENDER ((RO_MMIO_32)(MAILBOX_BASE + 0x14))
#define MAILBOX_READ_STATUS ((RO_MMIO_32)(MAILBOX_BASE + 0x18))
#define MAILBOX_READ_CONFIG ((RO_MMIO_32)(MAILBOX_BASE + 0x1C))

// Mailbox 1 Registers
#define MAILBOX_WRITE ((MMIO_32)(MAILBOX_BASE + 0x20))
#define MAILBOX_WRITE_PEEK ((MMIO_32)(MAILBOX_BASE + 0x30))
#define MAILBOX_WRITE_SENDER ((MMIO_32)(MAILBOX_BASE + 0x34))
#define MAILBOX_WRITE_STATUS ((MMIO_32)(MAILBOX_BASE + 0x38))
#define MAILBOX_WRITE_CONFIG ((MMIO_32)(MAILBOX_BASE + 0x3C))

#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

#define PARSE_SUCCESS 0x80000000
#define PARSE_FAILURE 0x80000001

#define EMPTY 0
#define REQUEST 0
#define END_TAG 0

/*
-> All the property tags;
-> in the future, the tags will have their own reference, so you know how to use them;
*/

enum propertyTags {
    FIRMWARE_REVISION = 0x00000001,
    BOARD_MODEL = 0x00010001,

    // ---------------------- //
    
    FRAMEBUFFER_ALLOCATE = 0x00040001,
    FRAMEBUFFER_RELEASE = 0x00048001,
    FRAMEBUFFER_BLANK = 0x00040002,
    FRAMEBUFFER_GET_PHYSICAL = 0x00040003,
    FRAMEBUFFER_TEST_PHYSICAL = 0x00044003,
    FRAMEBUFFER_SET_PHYSICAL = 0x00048003,
    FRAMEBUFFER_GET_VIRTUAL = 0x00040004,
    FRAMEBUFFER_TEST_VIRTUAL = 0x00044004,
    FRAMEBUFFER_SET_VIRTUAL = 0x00048004,
    FRAMEBUFFER_GET_DEPTH = 0x00040005,
    FRAMEBUFFER_TEST_DEPTH = 0x00044005,
    FRAMEBUFFER_SET_DEPTH = 0x00048005,
    FRAMEBUFFER_GET_PIXELORDER = 0x00040006,
    FRAMEBUFFER_TEST_PIXELORDER = 0x00044006,
    FRAMEBUFFER_SET_PIXELORDER = 0x00048006,
    FRAMEBUFFER_PITCH = 0x00040008,
    FRAMEBUFFER_GET_OFFSET = 0x00040009,
    FRAMEBUFFER_TEST_OFFSET = 0x00044009,
    FRAMEBUFFER_SET_OFFSET = 0x00048009,

    // ------------------------ //
    
};

/*
-> a basic message layout;
-> it is pretty basic, and it needs manual manipulation of tags;
-> i will change that though;
*/

typedef struct {
    size_t size;
    uint32 request;
    uint32 tags[];
} mailboxMessage;

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

void mailboxWrite(uintptr pointer) {
    while ((*MAILBOX_WRITE_STATUS & MAILBOX_FULL) != 0) { /* does nothing */ }
    *MAILBOX_WRITE = (pointer | 8);
}

// ------------------------------ //

/*
-> reads what VideoCore sent us back;
-> Function only returns the upper 28 bits from the register downshifted;
-> must be called so that VC responds to the sent message;
-> will return 0x80000002 if trying to read channel 7;
*/

uint32 mailboxRead() {
    while (true) {
        while ((*MAILBOX_READ_STATUS & MAILBOX_EMPTY) != 0) { /* does nothing */ }
        uint32 registerContents = *MAILBOX_READ;
        if ((registerContents & 0xF) == 8) return registerContents >> 4;
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

void BCMframebufferInit() {
    if (GlobalFramebuffer.pointer != 0) return;

    static mailboxMessage ALIGNED(16) messageBuffer = {
        .size = sizeof(messageBuffer) * 16,
        .request = REQUEST,
        .tags = {
            FRAMEBUFFER_SET_PHYSICAL,   // 0
            8,                          // 1
            REQUEST,                    // 2
            1920,                       // 3
            1080,                       // 4

            // ---------------------------- //

            FRAMEBUFFER_SET_VIRTUAL,    // 5
            8,                          // 6
            REQUEST,                    // 7
            1920,                       // 8
            1080,                       // 9

            // --------------------------- //

            FRAMEBUFFER_SET_DEPTH,      // 10
            4,                          // 11
            REQUEST,                    // 12
            16,                         // 13

            // --------------------------- //

            FRAMEBUFFER_SET_OFFSET,     // 14
            8,                          // 15
            REQUEST,                    // 16
            0,                          // 17
            0,                          // 18

            // -------------------------- //

            FRAMEBUFFER_ALLOCATE,       // 19
            8,                          // 20
            REQUEST,                    // 21
            16,                         // 22
            EMPTY,                      // 23

            // ------------------------- //

            FRAMEBUFFER_PITCH,          // 24
            4,                          // 25
            REQUEST,                    // 26
            EMPTY,                      // 27

            // ------------------------ //

            FRAMEBUFFER_SET_PIXELORDER, // 28
            4,                          // 29
            REQUEST,                    // 30
            1,                          // 31

            END_TAG                     // 32
        }
    };

    
    mailboxWrite((uintptr)&messageBuffer);
    mailboxRead();

    // Fills whatever VC sent back;
    GlobalFramebuffer.physicalWidth = messageBuffer.tags[3];
    GlobalFramebuffer.physicalHeight = messageBuffer.tags[4];
    GlobalFramebuffer.pitch = messageBuffer.tags[27];
    GlobalFramebuffer.pointer = messageBuffer.tags[22];
    GlobalFramebuffer.size = messageBuffer.tags[23];
}
