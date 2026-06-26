CC      = clang
TRIPLE  = x86_64-pc-win32-coff

CFLAGS = --target=$(TRIPLE) \
          -Wall -Werror -Wno-format \
          -mno-red-zone \
          -fno-stack-protector \
          -fshort-wchar \
          -Iinclude/edk2/MdePkg/Include \
          -Iinclude/edk2/MdePkg/Include/X64 \
          -Iinclude

AS      = llvm-mc
ASFLAGS = -triple=$(TRIPLE) -filetype=obj

LD      = lld-link
LDFLAGS = -subsystem:efi_application -nodefaultlib

SRCS = $(wildcard *.c)
ASMS = $(wildcard *.s)

OBJS = $(SRCS:.c=.o) $(ASMS:.s=.o)

TARGET = BOOTX64.EFI
all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -entry:EfiMain $^ -out:$@

clean:
	rm -rf *.o *.EFI $(IMAGE)

.PHONY: all run clean clean-all