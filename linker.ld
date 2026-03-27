ENTRY(_start)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x8000, LENGTH = 512K
    RAM (rwx) : ORIGIN = 0x8000, LENGTH = 2M
}

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
    } > RAM AT > FLASH
    	
    .bss : { 
        *(.bss) 
        *(COMMON)
    } > RAM 
}
