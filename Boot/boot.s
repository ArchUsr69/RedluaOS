.section .init
.globl _start

_start:

    /* Set up stack pointer */
    mov sp, #0x8000

    /* Call main */
    bl kernel_main

hang:
    b hang
