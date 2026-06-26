all:
	$(MAKE) -C kernel
	$(MAKE) -C bootloader

clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C bootloader clean