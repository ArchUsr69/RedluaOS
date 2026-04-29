.section .boot
.globl BOOT

/* defined by the linker script */
.extern STACK
.extern BSS_START
.extern BSS_END

BOOT:
    ldr x0, =STACK
    mov sp, x0

    ldr x0, =BSS_START
    ldr x1, =BSS_END

clearBss:
    cmp x0, x1
    bge armv8Init
    str xzr, [x0], #8
    b clearBss

hang:
    b hang
