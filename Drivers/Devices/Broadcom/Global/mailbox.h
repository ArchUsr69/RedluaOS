#include <types.h>
#include <utils.h>
#include <broadcom.h>

#ifndef MAILBOX_H
#define MAILBOX_H

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

void mailboxBufferNew();
void mailboxTagNew(enum Tag identifier, size_t valueCount, uint32 *values);
void mailboxBufferEnd();
uint32 mailboxTagRead(enum Tag identifier, size_t valueIndex);
uint32 mailboxSendMsg();

#endif
