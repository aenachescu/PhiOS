#ifndef PhiOS_InterruptDescriptorTable64
#define PhiOS_InterruptDescriptorTable64

#include "types.h"
#include "cpu.h"

struct IDT64_Entry
{
    uint16 lowOffset;
    uint16 selector;
    uint8 IST;
    uint8 type;
    uint16 middleOffset;
    uint32 highOffset;
    uint32 alwaysZero;
} __attribute__((packed));

struct IDT64
{
    uint16 limit;
    uint32 base;
} __attribute__((packed));

typedef void (*ISR64_PFN)(IntCpuState64*);

size_t IDT64_init();
size_t IDT64_SetEntry(/* TODO: find sizes for args */);
size_t IDT64_RegisterHandler(uint32 a_n, ISR64_PFN a_handler);
extern void IDT64_Load(uint64 a_table);

void IDT64_isrHandler(IntCpuState64 *a_state);
void IDT64_irqHandler(IntCpuState64 *a_state);

#endif
