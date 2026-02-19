# This Makefile is a disaster. It won't work, but i'll fix it sometimes else,
# i'm just not in the mood now

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBCOPY = arm-none-eabi-objcopy

CFLAGS = -ffreestanding -nostdlib -march=armv6
LDFLAGS = -T linker.ld

CSOURCE = Hardware/BCM2835/GPIO.c Kernel/kernel.c
ASSOURCE = Boot/boot.s
OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(CSOURCE)) \
	   $(patsubst %.s,$(BUILD_DIR)/%.o,$(ASSOURCE))

ELF = kernel.elf
IMAGE = kernel.img
BUILD_DIR = Build

all: $(IMAGE)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Assembly
$(BUILD_DIR)/%.o: $(ASSOURCE)/%.s | $(BUILD_DIR)
	$(AS) $< -o $@

# Turning C to Assembly
$(BUILD_DIR)/%.s: $(CSOURCE)/%.c
	$(CC) $(CFLAGS) -S $< -o $@

# Assembling
$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s | $(BUILD_DIR)
	$(AS) $< -o $@

# Linking
$(ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(IMAGE): $(ELF)
	$(OBJCOPY) -O binary $< $@

clean:
	rm -r $(BUILD_DIR) *.img *.elf
