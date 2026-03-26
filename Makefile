# ARCH and DEVICE variables are used to pick the correct files;
# MODEL Variable is sent to Init section to select the correct data;

# NOTE: DEVICE and ARCH variables are case sensitive;

ARCH ?= armv6
DEVICE ?= Broadcom
MODEL ?= BCM2835

# Architecture selection;

ifeq($(ARCH),armv6)
	# ------ Toolchain ------ #
	C_COMPILER = arm-none-eabi-gcc
	ASSEMBLER  = arm-none-eabi-as
	LINKER     = arm-none-eabi-ld
	OBJCOPY    = arm-none-eabi-objcopy

	BOOT_SECTION = Boot/armv6.s
	INIT_SECTION = Init/armv6.c
else
	$(error Unsuported architecture: $(ARCH))
endif

# Sources
SOURCE_DIRS = Kernel Libraries $(wildcard */$(DEVICE))
BUILD_DIR = Build

# Finds sources
C_SOURCE  := $(shell find $(SOURCE_DIRS) -name '*.c')
ASM_SOURCE := $(BOOT_SECTION)
HEADER_SOURCE := $(shell find $(SOURCE_DIRS) -type d -name Global)

# Flags
INCLUDE_FLAG := $(foreach dir,$(HEADER_SOURCE),-I$(dir))
C_FLAGS = -ffreestanding -nostdlib -march=$(ARCH) $(INCLUDE_FLAG) -D$(DEVICE) -D$(MODEL)
LD_FLAGS = -T linker.ld

# Generated intermediate assembly from C
C_ASM := $(patsubst %.c,$(BUILD_DIR)/%.s,$(C_SOURCE))

# Object files
C_OBJECTS  := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCE))
ASM_OBJECTS := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SOURCE))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

ELF   = kernel.elf
IMAGE = kernel.img

# Default target
all: $(IMAGE)

# Ensures build root exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --------------------------------------------------
# C -> Assembly (kept on purpose)
# --------------------------------------------------
$(BUILD_DIR)/%.s: %.c
	mkdir -p $(dir $@)
	$(C_COMPILER) $(C_FLAGS) -S $< -o $@

# --------------------------------------------------
# Generated Assembly -> Object
# --------------------------------------------------
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s
	$(ASSEMBLER) $< -o $@

# --------------------------------------------------
# Hand-written Assembly -> Object
# --------------------------------------------------
$(BUILD_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(ASSEMBLER) $< -o $@

# Linking
$(ELF): $(OBJECTS)
	$(LINKER) $(LD_FLAGS) $(OBJECTS) -o $@

$(IMAGE): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Cleanup
clean:
	rm -rf $(BUILD_DIR) $(IMAGE) $(ELF)

.PHONY: all clean

# for me only; you can ignore this. I didn't like typing those commands all the time;
copy_image:
	sudo mount /dev/mmcblk0p1 /mnt/SDCard
	sudo rm /mnt/SDCard/$(IMAGE)
	sudo cp $(IMAGE) /mnt/SDCard
	sudo umount /mnt/SDCard
