#include <vga/text_mode.h>

int drivers_test_func();

void kernel_main(void) {
    int a = drivers_test_func();
    VGA_Init();
    VGA_WriteString("Hello, PhiOS!\n");
    VGA_WriteString("Halt");

    return ;
}
