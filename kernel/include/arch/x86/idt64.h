#ifndef PhiOS_idt64
#define PhiOS_idt64

#include "types.h"
#include "cpu.h"

struct IDT_Entry64 {
    uint16 lowOffset;
    uint16 selector;
    uint8 IST;
    uint8 type;
    uint16 middleOffset;
    uint32 highOffset;
    uint32 alwaysZero;
} __attribute__((packed));
typedef struct IDT_Pointer64 IDT_Entry64;

struct IDT_Pointer64 {
    uint16 limit;
    uint32 base;
} __attribute__((packed));
typedef struct IDT_Pointer64 IDT_Pointer64;

typedef void (*ISRfunc64)(IntCpuState64*);

size_t IDT_init64();
size_t IDT_SetEntry64(/* TODO: find sizes for args */);
size_t IDT_RegisterHandler64(uint32 a_n, ISRfunc64 a_handler);
extern void IDT_Load64(uint64 a_table);

void IDT_isrHandler64(IntCpuState64 *a_state);
void IDT_irqHandler64(IntCpuState64 *a_state);

#endif
