ENTRY(BOOT)

SECTIONS {

    . = 0x8000;

    STACK = .;

    .text : {
        *(.boot)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    . = ALIGN(16);
    BSS_START = .;

    .bss : { 
        *(.bss) 
    }

    . = ALIGN(16);
    BSS_END = .;
}
