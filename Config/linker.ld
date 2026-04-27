ENTRY(_start)

SECTIONS {

    __STACK = .;
    .init 0x8000 : {
        *(.init)
    }

    .text : {
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
