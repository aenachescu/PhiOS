#include <vga/text_mode.h>

void kernel_main(void) {
    VGA_Init();
    VGA_WriteString("Hello, PhiOS!\n");
    VGA_WriteString("Halt");

    return ;
}
