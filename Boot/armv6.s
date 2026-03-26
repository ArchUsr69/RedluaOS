.section .init
.globl _start

_start:

    /* Set up stack pointer */
    mov sp, #0x8000

    /* Calls the actual Kernel */
    bl armv6Init
    
hang:
    b hang
