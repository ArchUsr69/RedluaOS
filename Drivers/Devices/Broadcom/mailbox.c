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

// this is the only channel in use;
#define CHANNEL 8

/*
-> EMPTY : waits for a response from VC;
-> REQUEST : tells VC it's a request;
-> END_TAG : kinda like a null-termination for the message buffer;
*/

#define EMPTY 0
#define REQUEST 0
#define END_TAG 0

// --------------------- //

/*
-> All the property tags identifiers;
-> in the future, the tags will have their own reference, so you know how to use them;
-> sorted after Branch;
-> For some tags, you must OR them with GET, TEST, or SET;

   Example: mailboxTagCreate(FRAMEBUFFER_PHYSICAL | SET, ...);
*/

#define GET 0x000
#define TEST 0x4000
#define SET 0x8000

enum Tag {

    FIRMWARE_REVISION = 0x1,
    BOARD_MODEL = 0x10001,
    BOARD_REVISION = 0x10002,
    BOARD_MAC_ADDRESS = 0x10003,
    BOARD_SERIAL = 0x10004,
    ARM_MEMORY = 0x10005,
    VC_MEMORY = 0x10006,
    GET_CLOCKS = 0x10007,
    COMMAND_LINE = 0x50001,
    DMA_CHANNELS = 0x60001,

    // ---------------------- //

    POWER_STATE = 0x20001,
    POWER_TIMING = 0x20002,

    // ---------------------- //

    CLOCK_STATE = 0x30001,
    CLOCK_RATE = 0x30002,
    LED_STATUS = 0x30041,
    CLOCK_RATE_MEASURED = 0x30047,
    CLOCK_RATE_MAX = 0x30004,
    CLOCK_RATE_MIN = 0x30007,
    CLOCK_TURBO = 0x30009,

    // ---------------------- //

    VOLTAGE = 0x30003,
    VOLTAGE_MAX = 0x30005,
    VOLTAGE_MIN = 0x30008,
    VOLTAGE_TEMP = 0x30006,
    VOLTAGE_TEMP_MAX = 0x3000A,
    MEMORY_ALLOCATE = 0x3000C,
    MEMORY_LOCK = 0x3000D,
    MEMORY_UNLOCK = 0x3000E,
    MEMORY_RELEASE = 0x3000F,
    EXECUTE_CODE = 0x30010,
    DISPAMNX_RESSOURCE_HANDLE = 0x30014,
    EDID_BLOCK = 0x30020,

    // ---------------------- //
    
    FRAMEBUFFER_ALLOCATE = 0x40001,
    FRAMEBUFFER_RELEASE = 0x48001,
    FRAMEBUFFER_BLANK = 0x40002,
    FRAMEBUFFER_PHYSICAL = 0x40003,
    FRAMEBUFFER_VIRTUAL = 0x40004,
    FRAMEBUFFER_DEPTH = 0x40005,
    FRAMEBUFFER_PIXELORDER = 0x40006,
    FRAMEBUFFER_PITCH = 0x40008,
    FRAMEBUFFER_OFFSET = 0x40009,
    FRAMEBUFFER_ALPHA_MODE = 0x40007,
    FRAMEBUFFER_OVERSCAN = 0x4000A,
    RGBA_PALETTE = 0x4000B,
    CURSOR_INFO = 0x8010,
    CURSOR_STATE = 0x8011,
    SCREEN_GAMMA = 0x8012
};

// ---------------------- //


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

uint32 ALIGNED(16) Buffer[64];
uint32 WordCount = 0;

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
    *MAILBOX_WRITE = ((uintptr)Buffer | CHANNEL);
    while (true) {
        while ((*MAILBOX_READ_STATUS & MAILBOX_EMPTY) != 0) { /* does nothing */ }
        uint32 registerContents = *MAILBOX_READ;
        if ((registerContents & CHANNEL) == CHANNEL) return Buffer[1];
    }
}

// -------------------------- //

/*
-> All values must already be initialized;
-> look down to see what values must be initialised;
*/

void BCMframebufferInit() {
    if (GlobalFramebuffer.pointer != 0) return;

    mailboxBufferNew();
    mailboxTagNew(FRAMEBUFFER_ALLOCATE, 2, (uint32[]){16, EMPTY});
    mailboxTagNew(FRAMEBUFFER_PHYSICAL | SET, 2, (uint32[]){GlobalFramebuffer.physicalWidth, GlobalFramebuffer.physicalHeight});
    mailboxTagNew(FRAMEBUFFER_VIRTUAL | SET, 2, (uint32[]){GlobalFramebuffer.virtualWidth, GlobalFramebuffer.virtualHeight});
    mailboxTagNew(FRAMEBUFFER_DEPTH | SET, 1, (uint32[]){GlobalFramebuffer.depth});
    mailboxTagNew(FRAMEBUFFER_PITCH | GET, 1, (uint32[]){EMPTY});
    mailboxTagNew(FRAMEBUFFER_OFFSET | SET, 2, (uint32[]){GlobalFramebuffer.virtual_X_Offset, GlobalFramebuffer.virtual_Y_Offset});
    mailboxTagNew(FRAMEBUFFER_PIXELORDER | SET, 1, (uint32[]){GlobalFramebuffer.pixelOrder});
    mailboxBufferEnd();
    mailboxSendMsg();

    // Fills whatever VC sent back;
    GlobalFramebuffer.physicalWidth = mailboxTagRead(FRAMEBUFFER_PHYSICAL | SET, 1);
    GlobalFramebuffer.physicalHeight = mailboxTagRead(FRAMEBUFFER_PHYSICAL | SET, 2);
    GlobalFramebuffer.pitch = mailboxTagRead(FRAMEBUFFER_PITCH | GET, 1);
    GlobalFramebuffer.pointer = mailboxTagRead(FRAMEBUFFER_ALLOCATE, 1);
    GlobalFramebuffer.size = mailboxTagRead(FRAMEBUFFER_ALLOCATE, 2);
}

// ------------------------- //
