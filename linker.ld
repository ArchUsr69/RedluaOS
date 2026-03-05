ENTRY(_start)

SECTIONS {

    .init 0x8000 : {
        *(.init)
    }

    .text : {
        *(.text*)
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }
	
    .bss : { 
        *(.bss*) 
        *(COMMON)
    }
}
