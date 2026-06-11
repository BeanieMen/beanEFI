SRC_DIR = src
BUILD_DIR = build

BOOT = $(SRC_DIR)/boot/boot
KERNEL = $(SRC_DIR)/kernel/kernel

ASM = nasm
QEMU = qemu-system-x86_64
CC = gcc
LD = ld
OBJCOPY = objcopy 

CFLAGS = -m32 -ffreestanding -fno-pie -fno-stack-protector -nostdlib
LDFLAGS = -m elf_i386 -T linker.ld


all: $(BUILD_DIR)/os-image.bin

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/boot.bin: $(BOOT).asm | $(BUILD_DIR)
	$(ASM) -f bin $(BOOT).asm -o $(BUILD_DIR)/boot.bin

$(BUILD_DIR)/kernel.o: $(KERNEL).c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(KERNEL).c -o $(BUILD_DIR)/kernel.o

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/kernel.o linker.ld | $(BUILD_DIR)
	$(LD) $(LDFLAGS) $(BUILD_DIR)/kernel.o -o $(BUILD_DIR)/kernel.elf

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf | $(BUILD_DIR)
	$(OBJCOPY) -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/os-image.bin: $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin | $(BUILD_DIR)
	cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/kernel.bin > $(BUILD_DIR)/os-image.bin


run: $(BUILD_DIR)/os-image.bin
	$(QEMU) -drive format=raw,file=$(BUILD_DIR)/os-image.bin

debug: $(BUILD_DIR)/os-image.bin
	$(QEMU) -s -S -drive format=raw,file=$(BUILD_DIR)/os-image.bin

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run debug clean