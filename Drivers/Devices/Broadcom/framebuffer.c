#include <types.h>
#include <utils.h>
#include <mailbox.h>
#include <framebuffer.h>

/*
-> All values must already be initialized;
-> look down to see what values must be initialised;
*/

void BCMframebufferInit() {
    if (Framebuffer.pointer != 0) return;

    mailboxBufferNew();
    mailboxTagNew(FRAMEBUFFER_ALLOCATE, 2, (uint32[]){16, EMPTY});
    mailboxTagNew(FRAMEBUFFER_PHYSICAL | SET, 2, (uint32[]){Framebuffer.physicalWidth, Framebuffer.physicalHeight});
    mailboxTagNew(FRAMEBUFFER_VIRTUAL | SET, 2, (uint32[]){Framebuffer.virtualWidth, Framebuffer.virtualHeight});
    mailboxTagNew(FRAMEBUFFER_DEPTH | SET, 1, (uint32[]){Framebuffer.depth});
    mailboxTagNew(FRAMEBUFFER_PITCH | GET, 1, (uint32[]){EMPTY});
    mailboxTagNew(FRAMEBUFFER_OFFSET | SET, 2, (uint32[]){Framebuffer.virtual_X_Offset, Framebuffer.virtual_Y_Offset});
    mailboxTagNew(FRAMEBUFFER_PIXELORDER | SET, 1, (uint32[]){Framebuffer.pixelOrder});
    mailboxBufferEnd();
    mailboxSendMsg();

    // Fills whatever VC sent back;
    Framebuffer.physicalWidth = mailboxTagRead(FRAMEBUFFER_PHYSICAL | SET, 1);
    Framebuffer.physicalHeight = mailboxTagRead(FRAMEBUFFER_PHYSICAL | SET, 2);
    Framebuffer.pitch = mailboxTagRead(FRAMEBUFFER_PITCH | GET, 1);
    Framebuffer.pointer = mailboxTagRead(FRAMEBUFFER_ALLOCATE, 1);
    Framebuffer.size = mailboxTagRead(FRAMEBUFFER_ALLOCATE, 2);
}

// ------------------------- //
