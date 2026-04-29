.section .text.boot
.globl _start

.extern __STACK
.extern __BSS_START
.extern __BSS_END

_start:

    /* Set up stack pointer */
    ldr r0, =__STACK
    mov sp, r0

    ldr r0, =__BSS_START
    ldr r1, =__BSS_END

clearBss:
    cmp r0, r1
    bge armv6Init
    str r2, [r0], #4
    b clearBss

hang:
    b hang
