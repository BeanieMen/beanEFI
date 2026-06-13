#include <stdint.h>
#include "vga/vga.h"

void kmain(void)
{
    vga_clear();
    vga_print("Beanie Man");
    
    // infinite loop so no weird bootloop
    for (;;)
    {
        __asm__ volatile("hlt");
    }
}