#include <stdint.h>
#include <stdbool.h>
#include <bcm2835.h>
#include <lowLevel.h>
#include <framebuffer.h>

/*
-> standard mailbox buffer structure; 
-> size must be calculated;
-> request must always be 0 initialized; VC will overwrite with 0x80000000 for success, 0x80000001 for failure;
*/

struct mailboxBuffer {
    uint32_t size;
    uint32_t request;
    uint32_t tags[64];
};

// ---------------------------- //

/*
-> sends a pointer to the Buffer to the MAILBOX_WRITE register;
-> the pointer must be 16 bytes aligned, since some genius decided it would be a
good idea to have the channel and pointer in the same register;
-> data must be already aligned to upper 28 bits before passed into the function;
-> the function must wait until the register is ready to be written to;

What the register expects:

bit [31-4] -> pointer
bit [4-0] -> channel 8 only (hardcoded)
*/

static void mailboxWrite(struct mailboxBuffer *pointer) {
    while (*MAILBOX_STATUS & MAILBOX_FULL) { /* spins */ }
    uint32_t register_contents = 0;
    writeField32(&register_contents, (uint32_t)pointer, 4, 28);
    writeField32(&register_contents, 8, 0, 4);
    *MAILBOX_WRITE = register_contents;
}

//------------------------------//

/*
-> reads from the MAILBOX_READ register;
-> Function only returns only the upper 28 bits from the register downshifted, so the upper 4 bits from the returned data will always be empty
*/

static uint32_t mailboxRead() {
    uint32_t register_contents;
    do {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY) { /* spins */ }
        register_contents = readField32(MAILBOX_READ, 4, 28);
    } while (readField32(MAILBOX_READ, 0, 4) != 8);
    return register_contents;
}

//------------------------------//

/*
-> initializes a framebuffer;
-> takes a framebuffer info struct pointer where it writes all the information about the framebuffer; 
*/

void mailboxFramebufferInit(struct framebuffer_metadata *pointer) {
    struct mailboxBuffer Buffer __attribute__((aligned(16)));

    Buffer.request = 0;

    //-------------------//

    Buffer.tags[0] = FRAMEBUFFER_ALLOCATE;
    Buffer.tags[1] = 8;
    Buffer.tags[2] = 0;
    Buffer.tags[3] = 16;
    Buffer.tags[4] = 0;

    //-------------------//

    Buffer.tags[5] = SET_PHYSICAL_SIZE;
    Buffer.tags[6] = 8;
    Buffer.tags[7] = 0;
    Buffer.tags[8] = 1080;
    Buffer.tags[9] = 780;

    //-------------------//

    Buffer.tags[10] = SET_VIRTUAL_SIZE;
    Buffer.tags[11] = 8;
    Buffer.tags[12] = 0;
    Buffer.tags[13] = 1080;
    Buffer.tags[14] = 780;

    //------------------//

    Buffer.tags[15] = SET_DEPTH;
    Buffer.tags[16] = 4;
    Buffer.tags[17] = 0;
    Buffer.tags[18] = 8;

    //------------------//

    Buffer.tags[19] = GET_PITCH;
    Buffer.tags[20] = 4;
    Buffer.tags[21] = 0;
    Buffer.tags[22] = 0;

    //------------------//

    Buffer.tags[23] = SET_VIRTUAL_OFFSET;
    Buffer.tags[24] = 8;
    Buffer.tags[25] = 0;
    Buffer.tags[26] = 0;
    Buffer.tags[27] = 0;

    //-----------------//

    Buffer.tags[28] = SCREEN_BLANK;
    Buffer.tags[29] = 4;
    Buffer.tags[30] = 0;
    Buffer.tags[31] = 1;

    Buffer.tags[32] = 0;
    Buffer.size = (2 + 33) * 4;

    mailboxWrite(&Buffer);
}

// ----------------------------- //
