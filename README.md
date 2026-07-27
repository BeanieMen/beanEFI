# beanEFI

beanEFI is a project i made to learn about the bootloader and how my linux boots up, it uses the edk II header files and ovfm uefi image to run 


---

## Demo & Screenshots

Here is `beanEFI` in action, drawing a custom font rendering "beanie" in a shifting rainbow color palette:

![beanEFI Scrolling Rainbow Demo](demo.gif)

*Static screenshot:*
![beanEFI UEFI Framebuffer Screenshot](screenshot.png)

---

## Architecture & Specifications

Clang builds a c program using edk2 uefi headers to get functions which the uefi gives and the bios runs the uefi stub

---

## 🚀 Getting Started

### Prerequisites

Ensure you have the following packages installed on your Linux system:

```sh
# On Debian/Ubuntu based systems:
sudo apt install clang lld qemu-system-x86_64 mtools dosfstools
```

```sh
# On Arch:
yay -S clang lld qemu-desktop mtools dosfstools
```

### Building and Running

1. **Clone the repository:**
   ```sh
   git clone https://github.com/BeanieMen/beanEFI.git
   cd beanEFI
   ```

2. **Build and launch in QEMU:**
   ```sh
   make run
   ```

---