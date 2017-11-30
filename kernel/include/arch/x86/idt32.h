#ifndef PhiOS_InterruptDescriptorTable32
#define PhiOS_InterruptDescriptorTable32

#include "include/types.h"
#include "include/errors.h"
#include "kernel/include/cpu.h"

#define IDT_ENTRIES 256

struct IDT32_Entry {
    uint16 lowOffset;
    uint16 selector;
    uint8 alwaysZero;
    uint8 type;
    uint16 highOffset;
} __attribute__((packed));

struct IDT32 {
    uint16 limit;
    uint32 base;
} __attribute__((packed));

typedef void (*ISR32_PFN)(IntCpuState32*);

uint32 IDT32_init();

uint32 IDT32_registerHandler(
    uint32 a_n, 
    ISR32_PFN a_handler
);

extern void isr_32_0 ();
extern void isr_32_1 ();
extern void isr_32_2 ();
extern void isr_32_3 ();
extern void isr_32_4 ();
extern void isr_32_5 ();
extern void isr_32_6 ();
extern void isr_32_7 ();
extern void isr_32_8 ();
extern void isr_32_9 ();
extern void isr_32_10();
extern void isr_32_11();
extern void isr_32_12();
extern void isr_32_13();
extern void isr_32_14();
extern void isr_32_15();
extern void isr_32_16();
extern void isr_32_17();
extern void isr_32_18();
extern void isr_32_19();
extern void isr_32_20();
extern void isr_32_21();
extern void isr_32_22();
extern void isr_32_23();
extern void isr_32_24();
extern void isr_32_25();
extern void isr_32_26();
extern void isr_32_27();
extern void isr_32_28();
extern void isr_32_29();
extern void isr_32_30();
extern void isr_32_31();
extern void isr_32_128();

extern void irq_32_0();
extern void irq_32_1();
extern void irq_32_2();
extern void irq_32_3();
extern void irq_32_4();
extern void irq_32_5();
extern void irq_32_6();
extern void irq_32_7();
extern void irq_32_8();
extern void irq_32_9();
extern void irq_32_10();
extern void irq_32_11();
extern void irq_32_12();
extern void irq_32_13();
extern void irq_32_14();
extern void irq_32_15();

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
