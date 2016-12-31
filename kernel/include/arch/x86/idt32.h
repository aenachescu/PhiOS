#ifndef PhiOS_idt32
#define PhiOS_idt32

#include "types.h"
#include "cpu.h"

#define IDT_ENTRIES 256

struct IDT_Entry32 {
    uint16 lowOffset;
    uint16 selector;
    uint8 alwaysZero;
    uint8 type;
    uint16 highOffset;
} __attribute__((packed));
typedef struct IDT_Entry32 IDT_Entry32;

struct IDT_Pointer32 {
    uint16 limit;
    uint32 base;
} __attribute__((packed));
typedef struct IDT_Pointer32 IDT_Pointer32;

typedef void (*ISRfunc32)(IntCpuState32*);

size_t IDT_init32();
size_t IDT_SetEntry32(uint32 a_n, uint32 a_base,
                    uint16 a_selector, uint8 a_flags);
size_t IDT_RegisterHandler32(uint32 a_n, ISRfunc32 a_handler);
extern void IDT_Load(uint32 a_table);

void IDT_isrHandler32(IntCpuState32 *a_state);
void IDT_irqHandler32(IntCpuState32 *a_state);

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

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
