.section .boot
.globl BOOT

/* defined by the linker script */
.extern STACK
.extern BSS_START
.extern BSS_END

BOOT:
    ldr r0, =STACK
    mov sp, r0

    ldr r0, =BSS_START
    ldr r1, =BSS_END

clearBss:
    cmp r0, r1
    bge armv6Init
    str r2, [r0], #4
    b clearBss

hang:
    b hang
