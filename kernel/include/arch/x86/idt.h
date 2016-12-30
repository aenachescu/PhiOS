#ifndef PhiOS_idt
#define PhiOS_idt

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

struct IDT_Pointer32 {
    uint16 limit;
    uint32 base;
} __attribute__((packed));
typedef struct IDT_Pointer32 IDT_Pointer32;

struct IDT_Pointer64 {
    uint16 limit;
    uint32 base;
} __attribute__((packed));
typedef struct IDT_Pointer64 IDT_Pointer64;

typedef void (*ISRfunc32)(IntCpuState32*);
typedef void (*ISRfunc64)(IntCpuState64*);

size_t IDT_init32();
size_t IDT_init64();
size_t IDT_SetEntry32(uint32 a_n, uint32 a_base,
                    uint16 a_selector, uint8 a_flags);
size_t IDT_SetEntry64(/* TODO: find sizes for args */);
size_t IDT_RegisterHandler32(uint32 a_n, ISRfunc32 a_handler);
size_t IDT_RegisterHandler64(uint32 a_n, ISRfunc64 a_handler);
extern void IDT_Load(uint32 a_table);

#endif
