#include <vga/text_mode.h>
#include <types.h>

void kernel_main(void) {
    VGA_Init();
    VGA_WriteString("Hello, PhiOS!\n");
    VGA_WriteString("Halt\n");

#if defined WORDSIZE && WORDSIZE == 32
    VGA_WriteString("32\n");
#endif

#if defined WORDSIZE && WORDSIZE == 64
    VGA_WriteString("64\n");
#endif

    return ;
}
