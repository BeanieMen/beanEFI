#include <stdint.h>

void kmain(void)
{
    volatile uint16_t *vga = (volatile uint16_t *)0xB8000;

    // clear screen (80 * 25)
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = (0x0F << 8) | ' ';  // white on black space
    }

    const char* msg = "BEANIE";

    int row = 12;
    int col = (80 - 6) / 2;

    int index = row * 80 + col;

    for (int i = 0; msg[i] != '\0'; i++) {
        vga[index + i] = (0x0F << 8) | msg[i];
    }
}