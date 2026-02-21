# This Bitch still won't work. I Overcomplicated this shit. Just look at it. It's an ugly piece of shit.
# i can't believe the hardest part of this project is to be able to compile it automatically.
# Guess i'll do it by hand

# Toolchain in use
C_COMPILER = arm-none-eabi-gcc
ASSEMBLER = arm-none-eabi-as
LINKER = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

# Toolchain Flags
C_FLAGS = -ffreestanding -nostdlib -march=armv6
LD_FLAGS = -T linker.ld

SOURCE_DIR = Kernel \
			 Hardware \
			 Boot \
			 Libraries

# Finds all the C / Assembly files
C_SOURCE = $(wildcard $(SOURCE_DIR)/*.c)
C_ASM = $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.s,$(C_SOURCE))
C_OBJECTS = $(patsubst $(BUILD_DIR)/%.s,$(BUILD_DIR)/%.o,$(C_ASM))

ASM_SOURCE = $(wildcard $(SOURCE_DIR)/*.s)
ASM_OBJECTS = $(patsubst $(SOURCE_DIR)/%.s,$(BUILD_DIR)/%.o,$(ASM_SOURCE))

OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)
ELF = kernel.elf
IMAGE = kernel.img

BUILD_DIR = Build

#------Default------#

all: $(IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Turning C to Assembly
$(BUILD_DIR)/%.s: $(SOURCE_DIR)/%.c | $(BUILD_DIR)
	$(C_COMPILER) $(C_FLAGS) -S $< -o $@

# Assembling C-generated Assembly
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s | $(BUILD_DIR)
	$(ASSEMBLER) $< -o $@

# Assembling Written Assembly
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.s | $(BUILD_DIR)
	$(ASSEMBLER) $< -o $@
	
# Linking
$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(IMAGE): $(ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -r $(BUILD_DIR) kernel.img kernel.elf
