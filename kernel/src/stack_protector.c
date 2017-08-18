#include "include/types.h"
#include "include/cpu.h"

#include "util/kstdlib/include/kstdio.h"

// Temporary value until the random one is generated
uint32 __stack_chk_guard = 0xdeadbeef;

__attribute__((noreturn))
void __stack_chk_fail(void)
{
    kprintf("[PANIC] Stack smashing detected!\n");
    freezeCpu();
}

__attribute__ ((noreturn))
void __stack_chk_fail_local (void)
{
    __stack_chk_fail();
}
