.section .init
.globl _start

.extern __STACK
.extern __BSS_START
.extern __BSS_END

_start:

    /* Set up stack pointer */
    mov sp, =__STACK

    /* Clears .bss */
    ldr r0, =__BSS_START
    ldr r1, =__BSS_END
    mov r2, #0

clearBss:
    cmp r0, r1
    bge armv6Init
    str r2, [r0], #4
    b clearBss

hang:
    b hang
