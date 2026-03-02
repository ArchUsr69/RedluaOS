# ===== Toolchain =====
C_COMPILER = arm-none-eabi-gcc
ASSEMBLER  = arm-none-eabi-as
LINKER     = arm-none-eabi-ld
OBJCOPY    = arm-none-eabi-objcopy

# ===== Sources =====
SOURCE_DIRS = Kernel Hardware Boot Libraries
BUILD_DIR = Build

# ===== Find sources (recursive) =====
C_SOURCE  := $(shell find $(SOURCE_DIRS) -name '*.c')
ASM_SOURCE := $(shell find $(SOURCE_DIRS) -name '*.s')
HEADER_FILES := $(shell find $(SOURCE_DIRS) -name '*.h')

# ===== Flags =====
INCLUDE_FLAG := $(patsubst %, -I%,$(HEADER_FILES))
C_FLAGS = -ffreestanding -nostdlib -march=armv6 $(INCLUDE_FLAG)
LD_FLAGS = -T linker.ld

# ===== Generated intermediate assembly from C =====
C_ASM := $(patsubst %.c,$(BUILD_DIR)/%.s,$(C_SOURCE))

# ===== Object files =====
C_OBJECTS  := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCE))
ASM_OBJECTS := $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASM_SOURCE))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

ELF   = kernel.elf
IMAGE = kernel.img

# ===== Default target =====
all: $(IMAGE)

# ===== Ensure build root exists =====
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --------------------------------------------------
# C → Assembly (kept on purpose)
# --------------------------------------------------
$(BUILD_DIR)/%.s: %.c
	mkdir -p $(dir $@)
	$(C_COMPILER) $(C_FLAGS) -S $< -o $@

# --------------------------------------------------
# Generated Assembly → Object
# --------------------------------------------------
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s
	$(ASSEMBLER) $< -o $@

# --------------------------------------------------
# Hand-written Assembly → Object
# --------------------------------------------------
$(BUILD_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(ASSEMBLER) $< -o $@

# ===== Linking =====
$(ELF): $(OBJECTS)
	$(LINKER) $(LD_FLAGS) $(OBJECTS) -o $@

$(IMAGE): $(ELF)
	$(OBJCOPY) -O binary $< $@

# ===== Cleanup =====
clean:
	rm -rf $(BUILD_DIR) $(IMAGE) $(ELF)

.PHONY: all clean
