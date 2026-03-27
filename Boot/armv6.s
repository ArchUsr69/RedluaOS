.section .init
.globl _boot

_start:

    /* Set up stack pointer */
    mov sp, #0x8000

    /* Calls Init */
    bl armv6Init
    
hang:
    b hang
