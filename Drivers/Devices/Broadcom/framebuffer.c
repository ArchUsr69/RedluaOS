#include <types.h>
#include <utils.h>
#include <mailbox.h>
#include <framebuffer.h>

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
