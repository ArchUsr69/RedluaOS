ENTRY(_start)

SECTIONS {

    . = 0x8000;

    __STACK = .;

    .text : {
        *(.text.boot)
        *(.text*)
    }

    .rodata : {
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    . = ALIGN(16);
    __BSS_START = .;

    .bss : { 
        *(.bss) 
        *(COMMON)
    }

    . = ALIGN(16);
    __BSS_END = .;
}
