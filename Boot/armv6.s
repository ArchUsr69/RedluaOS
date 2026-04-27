.section .init
.globl _start
.extern __bss_start
.extern __bss_end

_start:

    /* Set up stack pointer */
    mov sp, #0x8000

    /* Clears .bss */
    ldr r0, =__bss_start
    ldr r1, =__bss_end
    mov r2, #0

clearBss:
    cmp r0, r1
    bge armv6Init
    str r2, [r0], #4
    b clearBss

hang:
    b hang
