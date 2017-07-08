#include "arch/x86/pit.h"
#include "arch/x86/asm_io.h"
#include "cpu.h"
#include "arch/x86/idt.h"

/*
 * The interval time (in ms) in which it's called helper_pitHandler().
 */
static size_t g_time;

static void helper_pitHandler(__attribute__((unused)) CpuState *a_state)
{
    return;
}

size_t PIT_init(uint16 a_divisor)
{
    if (a_divisor == 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    g_time = 1000 / (OSCILLATOR_FREQUENCY / a_divisor);
    if (g_time == 0)
    {
        return ERROR_INVALID_PARAMETER;
    }

    IDT_registerHandler(IRQ0, helper_pitHandler);

    io_outb(0x43, 0x36);

    uint8 low = (uint8) (a_divisor & 0xFF);
    uint8 high = (uint8) ((a_divisor >> 8) & 0xFF);

    io_outb(0x40, low);
    io_outb(0x40, high);

    return ERROR_SUCCESS;
}
