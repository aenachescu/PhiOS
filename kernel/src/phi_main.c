#include "drivers/keyboard/include/keyboard.h"
#include "util/kstdlib/include/kstdio.h"

// TODO: remove this when tasks are available
size_t g_userStack[2048]; // temporary user mode

void user_main()
{
    kprintf("Hello, world!\n");
    kprintf("> ");
    while (1)
    {
        char c = keyboard_readKey();
        if (c == '\n')
        {
            kprintf("\n> ");
        }
        else
        {
            kprintf("%c", c);
        }
    }
}

void kernel_main()
{
    return;
}
