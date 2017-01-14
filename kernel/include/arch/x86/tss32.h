#ifndef PhiOS_TaskStateSegment32
#define PhiOS_TaskStateSegment32

#include "types.h"

struct TSS32_Entry
{
    uint32 prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
	uint32 esp0;       // The stack pointer to load when we change to kernel mode.
	uint32 ss0;        // The stack segment to load when we change to kernel mode.
	uint32 esp1;       // Unused...
	uint32 ss1;
	uint32 esp2;
	uint32 ss2;
	uint32 cr3;
	uint32 eip;
	uint32 eflags;
	uint32 eax;
	uint32 ecx;
	uint32 edx;
	uint32 ebx;
	uint32 esp;
	uint32 ebp;
	uint32 esi;
	uint32 edi;
	uint32 es;         // The value to load into ES when we change to kernel mode.
	uint32 cs;         // The value to load into CS when we change to kernel mode.
	uint32 ss;         // The value to load into SS when we change to kernel mode.
	uint32 ds;         // The value to load into DS when we change to kernel mode.
	uint32 fs;         // The value to load into FS when we change to kernel mode.
	uint32 gs;         // The value to load into GS when we change to kernel mode.
	uint32 ldt;        // Unused...
	uint16 trap;
    uint16 iomap_base;
} __attribute__((packed));
typedef struct TSS32_Entry TSS32_Entry;

size_t TSS32_init(uint16 ss0, uint32 esp0);
size_t TSS32_setKernelStack(uint32 a_esp);

extern void TSS32_Load();

#endif
