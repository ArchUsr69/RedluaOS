.section .init
.globl _start
.extern __bss_start
.extern __bss_end

_start:

    /* Set up stack pointer */
    mov sp, #0x8000

    /* Clears .bss */
    ldr x0, =__bss_start
    ldr x1, =__bss_end
    mov x2, #0

clearBss:
    cmp x0, x1
    bge armv8Init
    str xzr, [x0], #8
    b clearBss

hang:
    b hang
