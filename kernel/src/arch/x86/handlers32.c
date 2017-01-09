#include "arch/x86/idt32.h"
#include "arch/x86/handlers32.h"
#include "kstdio.h"
#include "cpu.h"
#include "vga/text_mode.h"

void handlers32_default(IntCpuState32 *a_state)
{
    VGA_WriteString("[");
    VGA_WriteColoredString("PANIC", VGA_ColorBlack, VGA_ColorRed);
    VGA_WriteString("]");
    VGA_WriteString(" Interrupt is not supported.\n");
    cpu_printState(a_state);
    stopCpu();
}
