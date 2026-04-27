.section .init
.globl _start

.extern __STACK
.extern __BSS_START
.extern __BSS_END

_start:

    /* Set up stack pointer */
    mov sp, =__STACK

    /* Clears .bss */
    ldr x0, =__BSS_START
    ldr x1, =__BSS_END
    mov x2, #0

clearBss:
    cmp x0, x1
    bge armv8Init
    str xzr, [x0], #8
    b clearBss

hang:
    b hang
