#include <types.h>
#include <utils.h>
#include <broadcom.h>
#include <framebuffer.h>

#define MAILBOX_BASE (PERIPHERAL_BASE + 0xB880)

// Mailbox 0 Registers (VC -> ARM);
#define MAILBOX_READ ((RO_MMIO_32)(MAILBOX_BASE + 0x00))
#define MAILBOX_READ_PEEK ((RO_MMIO_32)(MAILBOX_BASE + 0x10))
#define MAILBOX_READ_SENDER ((RO_MMIO_32)(MAILBOX_BASE + 0x14))
#define MAILBOX_READ_STATUS ((RO_MMIO_32)(MAILBOX_BASE + 0x18))
#define MAILBOX_READ_CONFIG ((RO_MMIO_32)(MAILBOX_BASE + 0x1C))

// Mailbox 1 Registers (ARM -> VC);
#define MAILBOX_WRITE ((MMIO_32)(MAILBOX_BASE + 0x20))
#define MAILBOX_WRITE_PEEK ((MMIO_32)(MAILBOX_BASE + 0x30))
#define MAILBOX_WRITE_SENDER ((MMIO_32)(MAILBOX_BASE + 0x34))
#define MAILBOX_WRITE_STATUS ((MMIO_32)(MAILBOX_BASE + 0x38))
#define MAILBOX_WRITE_CONFIG ((MMIO_32)(MAILBOX_BASE + 0x3C))

// just some status flags;
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000
#define PARSE_SUCCESS 0x80000000
#define PARSE_FAILURE 0x80000001

// for the property channel; deliver better readability;
#define EMPTY 0
#define REQUEST 0
#define END_TAG 0

/*
-> All the property tags identifiers;
-> in the future, the tags will have their own reference, so you know how to use them;
-> sorted after Branch;
*/

enum tags {
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
};

// ----------------------------------- //


/*
-> the message Buffer that will be sent;
-> structure of a BUFFER:

  [BUFFER_SIZE]    // number of 32-bit numbers IN THE ENTIRE BUFFER
  [REQUEST_CODE]   // always set to 0; VC will rewrite it;
  [TAG]              
  [TAG]
  ....
  [END_TAG]

-> structre of a TAG:

  [IDENTIFIER]
  [VALUES_BYTE_COUNT]
  [REQUEST_CODE]
  [VALUE]
  ......

-> a Tag may have more than 1 value;
-> some Values must be replaced by VC. You MUST allocate space for them inside the TAG in order to get a response;
*/

uint32 ALIGNED(16) Buffer[64];
uint32 wordOffset = 0;

void mailboxBufferNew() {
    memorySet(&Buffer, 0, sizeof(Buffer));
    Buffer[1] = REQUEST;
    wordOffset = 2;
}

void mailboxTagNew(enum tags identifier, size_t valueCount, uint32 *values) {
    Buffer[wordOffset++] = identifier;
    Buffer[wordOffset++] = (valueCount * 4);
    Buffer[wordOffset++] = REQUEST;
    for (uint8 valueOffset = 0; valueOffset < valueCount; valueOffset++) {
        Buffer[wordOffset++] = values[valueOffset];
    }
}

void mailboxBufferEnd() {
    Buffer[0] = wordOffset;
    Buffer[wordOffset] = END_TAG;
}

uint32 mailboxTagRead(enum tags identifier, size_t valueIndex) {
    uint8 offset = 0;
    while (Buffer[offset] != identifier) {
        if (offset++ > 64) return 0;
    }
    if ((valueIndex * 4) > Buffer[offset + 1]) return 0;
    return Buffer[(offset + 3) + valueIndex];
}

// -------------------------------- //

/*
-> sends a pointer of the message buffer to VideoCore;
-> the pointer *MUST* be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> the function must wait until the register is ready to be written to;

   What the register expects:

   bit [31-4] -> pointer;
   bit [3-0] -> channel;

-> after sending, it immediately reads back to get a response;
*/

uint32 mailboxSendMsg() {
    while ((*MAILBOX_WRITE_STATUS & MAILBOX_FULL) != 0) { /* does nothing */ }
    *MAILBOX_WRITE = ((uintptr)&Buffer | 8);
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
    
    mailboxBufferNew();
    mailboxTagNew(FRAMEBUFFER_ALLOCATE, 2, (uint32[]){16, EMPTY});
    mailboxTagNew(FRAMEBUFFER_SET_PHYSICAL, 2, (uint32[]){GlobalFramebuffer.physicalWidth, GlobalFramebuffer.physicalHeight});
    mailboxTagNew(FRAMEBUFFER_SET_VIRTUAL, 2, (uint32[]){GlobalFramebuffer.virtualWidth, GlobalFramebuffer.virtualHeight});
    mailboxTagNew(FRAMEBUFFER_SET_DEPTH, 1, (uint32[]){GlobalFramebuffer.depth});
    mailboxTagNew(FRAMEBUFFER_PITCH, 1, (uint32[]){EMPTY});
    mailboxTagNew(FRAMEBUFFER_SET_OFFSET, 2, (uint32[]){GlobalFramebuffer.virtual_X_Offset, GlobalFramebuffer.virtual_Y_Offset});
    mailboxTagNew(FRAMEBUFFER_SET_PIXELORDER, 1, (uint32[]){GlobalFramebuffer.pixelOrder});
    mailboxBufferEnd();
    mailboxSendMsg();

    // Fills whatever VC sent back;
    GlobalFramebuffer.physicalWidth = mailboxTagRead(FRAMEBUFFER_SET_PHYSICAL, 1);
    GlobalFramebuffer.physicalHeight = mailboxTagRead(FRAMEBUFFER_SET_PHYSICAL, 2);
    GlobalFramebuffer.pitch = mailboxTagRead(FRAMEBUFFER_PITCH, 1);
    GlobalFramebuffer.pointer = mailboxTagRead(FRAMEBUFFER_ALLOCATE, 1);
    GlobalFramebuffer.size = mailboxTagRead(FRAMEBUFFER_ALLOCATE, 2);
}
