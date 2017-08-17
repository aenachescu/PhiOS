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

uint32 IDT64_init();

uint32 IDT64_setEntry(/* TODO: find sizes for args */);

uint32 IDT64_registerHandler(
    uint32 a_n, 
    ISR64_PFN a_handler
);

extern void IDT64_load(
    uint64 a_table
);

void IDT64_isrHandler(
    IntCpuState64 *a_state
);

void IDT64_irqHandler(
    IntCpuState64 *a_state
);


#define IRQ0 32 // Programmable Interrupt Timer Interrupt (handled)
#define IRQ1 33 // Keyboard Interrupt (handled)
#define IRQ2 34 // Cascade (used internally by the two PICs. never raised)
#define IRQ3 35 // COM2 (if enabled)
#define IRQ4 36 // COM1 (if enabled)
#define IRQ5 37 // LPT2 (if enabled)
#define IRQ6 38 // Floppy Disk (handled)
#define IRQ7 39 // LPT1 / Unreliable "spurious" interrupt (usually)
#define IRQ8 40 // CMOS real-time clock (if enabled)
#define IRQ9 41 // Free for peripherals / legacy SCSI / NIC
#define IRQ10 42 // Free for peripherals / SCSI / NIC
#define IRQ11 43 // Free for peripherals / SCSI / NIC
#define IRQ12 44 // PS2 Mouse
#define IRQ13 45 // FPU / Coprocessor / Inter-processor
#define IRQ14 46 // Primary ATA Hard Disk
#define IRQ15 47 // Secondary ATA Hard Disk

#endif
