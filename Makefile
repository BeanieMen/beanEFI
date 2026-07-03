BOOTLOADER_DIR := bootloader
ESP_IMAGE := esp.img

OVMF_CODE := OVMF_CODE.4m.fd
OVMF_VARS := OVMF_VARS.4m.fd

.PHONY: all bootloader disk run clean

all: bootloader

bootloader:
	$(MAKE) -C $(BOOTLOADER_DIR)

disk: bootloader
	rm -f $(ESP_IMAGE)
	truncate -s 64M $(ESP_IMAGE)
	mkfs.fat -F32 $(ESP_IMAGE)
	mmd -i $(ESP_IMAGE) ::/EFI ::/EFI/BOOT
	mcopy -i $(ESP_IMAGE) $(BOOTLOADER_DIR)/BOOTX64.EFI ::/EFI/BOOT/

run: disk
	qemu-system-x86_64 \
		-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
		-drive if=pflash,format=raw,file=$(OVMF_VARS) \
		-drive format=raw,file=$(ESP_IMAGE)

clean:
	$(MAKE) -C $(BOOTLOADER_DIR) clean
	rm -f $(ESP_IMAGE)