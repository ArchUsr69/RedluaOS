ENTRY(_start)

SECTIONS {

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
    __bss_start = .;
    .bss : { 
        *(.bss) 
        *(COMMON)
    }
    . = ALIGN(16);
    __bss_end = .;
}
