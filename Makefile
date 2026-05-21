include Config/config.mk

BUILD_DIR = Build
ELF = Image/kernel.elf
IMAGE = Image/kernel.bin

ifdef ARCH

# Source directories
SOURCE_DIRS := Kernel redlib Userspace $(DEVICE_DIR)

# Finds source files
C_SOURCE  := $(shell find $(SOURCE_DIRS) -name '*.c')
ASM_SOURCE := $(BOOT_SECTION)
HEADER_SOURCE := Global redlib

# Flags
INCLUDE_FLAG := $(foreach dir,$(HEADER_SOURCE),-I$(dir))
C_FLAGS := -ffreestanding -nostdlib $(INCLUDE_FLAG) -D$(DEVICE) -D$(MODEL)
LD_FLAGS := -T Config/linker.ld

# Generated intermediate assembly from C
C_ASM := $(patsubst %.c,$(BUILD_DIR)/%.s,$(C_SOURCE))

# Object files
C_OBJECTS  := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCE))
ASM_OBJECTS := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SOURCE))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

endif

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

# for me only; you can ignore this. I didn't like typing those commands all the time;
cp:
	sudo mount /dev/mmcblk0p1 /mnt/SDCard
	sudo cp $(IMAGE) /mnt/SDCard
	sudo umount /mnt/SDCard
