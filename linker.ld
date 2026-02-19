SECTIONS {

# as a temporary measure, i put the startup code in the .init section; idk, it just worked
# like this, so i just leaved it like this. Took the idea from another Project i found

	.init 0x8000 : { *(.init) }
	
	.text : { *(.text) }
	
	.data : { *(.data) }

	/DISCARD/ : { *(*) }
}
