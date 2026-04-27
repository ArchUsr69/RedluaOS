SUPPORTED_ARCH := armv6 armv8
SUPPORTED_DEVICE := Broadcom

ifndef ARCH
	$(error Unspecified Architecture)
else ifndef DEVICE
	$(error Unspecified Device family)
else ifndef MODEL 
	$(error Unspecified Device Model)
endif

DEVICE_DIR := $(shell find Drivers -type d -name $(DEVICE))

ifeq ($(DEVICE_DIR),)
	$(error Unsuported Device family. Check for typos and Lower/Uppercase correctness)
endif

ifeq ($(filter $(ARCH), $(SUPPORTED_ARCH)),)
	$(error Unsuported or invalid Architecture. Check for typos)
else

	BOOT_SECTION := Boot/$(ARCH).s
	INIT_SECTION := Init/$(ARCH).c

	ifeq ($(ARCH), armv6)
		C_COMPILER := arm-none-eabi-gcc
		ASSEMBLER  := arm-none-eabi-as
		LINKER     := arm-none-eabi-ld
		OBJCOPY    := arm-none-eabi-objcopy

	else ifeq ($(ARCH), armv8)
		C_COMPILER := gcc
		ASSEMBLER := as
		LINKER := ld
		OBJCOPY := objcopy
	endif
endif