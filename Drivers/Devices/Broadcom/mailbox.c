#include <types.h>
#include <utils.h>
#include <broadcom.h>
#include <mailbox.h>

#ifdef BCM2712
    #define MAILBOX_BASE (PERIPHERAL_BASE + 0x13880)
#else
    #define MAILBOX_BASE (PERIPHERAL_BASE + 0xB880)
#endif

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

// this is the only channel in use;
#define CHANNEL 8

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

   *IMPORTANT*
-> a Tag may have more than 1 value;
-> some Values must be replaced by VC. You MUST allocate space for them inside the TAG using EMPTY;
-> valueCount starts at 1;
-> valueIndex starts with 1;

-> You may ask yourself; "Why must i multiply the wordcount with 16??" I have no fucking idea.
-> Just know, it wouldn't work if it wasn't multipllied by 16, so just consider it a "magic number"
*/

static uint32 ALIGNED(16) Buffer[64];
static uint32 WordCount = 0;

void mailboxBufferNew() {
    memorySet(Buffer, 0, sizeof(Buffer));
    Buffer[1] = REQUEST;
    WordCount = 2;
}

void mailboxTagNew(enum Tag identifier, size_t valueCount, uint32 *values) {
    if (values == 0 || valueCount > 4 || (WordCount + 3) >= (sizeof(Buffer) - valueCount)) return;

    Buffer[WordCount++] = identifier;
    Buffer[WordCount++] = valueCount * INT32_BYTES;
    Buffer[WordCount++] = REQUEST;

    for (size_t valueOffset = 0; valueOffset < valueCount; valueOffset++) {
        Buffer[WordCount++] = values[valueOffset];
    }
}

void mailboxBufferEnd() {
    if (WordCount >= (sizeof(Buffer) - 1)) return;

    Buffer[0] = (WordCount * 16);
    Buffer[WordCount] = END_TAG;
}

uint32 mailboxTagRead(enum Tag identifier, size_t valueIndex) {
    if (valueIndex == 0 || valueIndex > 4) return 0;
    size_t offset = 2;

    while (offset < WordCount) {
        if (Buffer[offset] != identifier) {
            size_t tagOffset = 3 + (Buffer[offset + 1] / 4);
            offset += tagOffset;
        } else if (Buffer[offset] == identifier) return Buffer[(offset + 2) + valueIndex];
    }
}

// -------------------------- //

/*
-> sends a pointer of the message buffer to VideoCore;
-> the pointer *MUST* be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;

   What the register expects:

   bit [31-4] -> pointer;
   bit [3-0] -> channel;

-> after sending, it immediately reads back to get a response;
*/

uint32 mailboxSendMsg() {
    while ((*MAILBOX_WRITE_STATUS & MAILBOX_FULL) != 0) { /* does nothing */ }
    *MAILBOX_WRITE = ((uint32)Buffer | CHANNEL);

    while (true) {
        while ((*MAILBOX_READ_STATUS & MAILBOX_EMPTY) != 0) { /* does nothing */ }
        uint32 registerContents = *MAILBOX_READ;
        if ((registerContents & CHANNEL) == CHANNEL) return Buffer[1];
    }
}

// -------------------------- //
