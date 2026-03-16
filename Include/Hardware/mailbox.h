#include <stdint.h>
#include <stddef.h>
// ----------------- //
#include <framebuffer.h>

#ifndef MAILBOX_H
#define MAILBOX_H

extern enum mailboxChannels channel;

void mailboxWrite(uintptr_t pointer, enum mailboxChannels channel, size_t bufferSize);
uint32_t mailboxRead(enum mailboxChannels channel);
void mailboxFramebufferInit(struct framebufferMetadata *framebufferMetadata);

#endif
