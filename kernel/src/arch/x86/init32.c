#include "kernel/include/arch/x86/init32.h"
#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/arch/x86/paging/paging.h"

#include "kernel/include/memory/paa.h"
#include "kernel/include/memory/pmm.h"
#include "kernel/include/memory/bitmap_pma.h"

#include "drivers/video/include/vga/text_mode.h"

#include "util/kstdlib/include/kstdio.h"

#include "kernel/include/multiboot2.h"

#include "cpu.h"

struct KernelArea g_kernelArea;

extern uint32 linker_kernelStart;
extern uint32 linker_kernelEnd;
extern uint32 linker_textStart;
extern uint32 linker_textEnd;
extern uint32 linker_rodataStart;
extern uint32 linker_rodataEnd;
extern uint32 linker_dataStart;
extern uint32 linker_dataEnd;
extern uint32 linker_bssStart;
extern uint32 linker_bssEnd;
extern uint32 linker_gotStart;
extern uint32 linker_gotEnd;

#define FRAME_SIZE 4096

// TODO: remove this when tasks are available
size_t g_kernelStack[2048]; // temporary kernel mode

struct BitmapPMA *g_PMAVM;
uint32 g_PMAVM_num = 0;
struct Paging g_kernelPaging;

extern struct PMA *g_allocators;

static __attribute__ ((unused)) void printKernelArea()
{
    kprintf("TextStart: %x TextEnd: %x\n",
        g_kernelArea.textStartAddr, g_kernelArea.textEndAddr);

    kprintf("RodataStart: %x RodataEnd: %x\n",
        g_kernelArea.rodataStartAddr, g_kernelArea.rodataEndAddr);

    kprintf("DataStart: %x DataEnd: %x\n",
        g_kernelArea.dataStartAddr, g_kernelArea.dataEndAddr);

    kprintf("BssStart: %x BssEnd: %x\n",
        g_kernelArea.bssStartAddr, g_kernelArea.bssEndAddr);

    kprintf("Placement address: %x\n", g_kernelArea.endPlacementAddr);
}

void detectSMBios()
{
    uint8 *p = (uint8*) 0xF0000;
    uint8 length = 0;

    while (p < (uint8*) 0x100000) {
        if (p[0] == '_' && p[1] == 'S' && p[2] == 'M' && p[3] == '_') {
            length = p[5];
            uint8 checksum = 0;
            for (uint8 i = 0; i < length; i++) {
                checksum += p[i];
            }
            if (checksum == 0) {
                kprintf("Found SMBios32 at address: %p\n", p);
                kprintf("SMBios32 info: majorVersion[%u], minorVersion[%u], revision[%u]\n", (uint32)p[6], (uint32)p[7], (uint32)p[30]);
                break;
            }
        }
        p += 16;
    }

    if (p >= (uint8*) 0x100000) {
        kprintf("NOT found SMBios32\n");
        return;
    }
}

static void adjust_got(uint32 a_offset)
{
    uint32 begin = (uint32) &linker_gotStart;
    uint32 end = (uint32) &linker_gotEnd;
    uint32 num = (end - begin) / 4;
    uint32 *ptr = (uint32*) begin;

    for (uint32 i = 0; i < num; i++) {
        ptr[i] += a_offset;
    }
}

void adjustPointers()
{
    uint32 offset = 0xBFF00000;

    PMM_adjustPfn(offset);

    for (size_t i = 0; i < g_PMAVM_num; i++) {
        g_PMAVM[i].bitmap = (size_t*) ((size_t)g_PMAVM[i].bitmap + offset);
    }

    g_allocators = (struct PMA*) ((size_t)g_allocators + offset);

    adjust_got(offset);
}

uint32 init_init32(
    uint32 mboot2Magic,
    uint32 mboot2Addr)
{
    // Inits VGA
    VGA_Init();
    kprintf("PhiOS v0.0.1 32-bit\n");

    if (mboot2Magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        VGA_WriteString("[PANIC] GRUB not multiboot2");
        return ERROR_NOT_FOUND;
    }

    if (mboot2Addr & 7) {
        VGA_WriteString("[PANIC] Multiboot2 structure is not aligned");
        return ERROR_UNALIGNED_ADDRESS;
    }

    VGA_WriteString("GRUB multiboot2\n");

    detectSMBios();

    // Iterate over tags and collect info
    uint64 memoryEnd = 0x0;
    struct {
        uint64 startAddr;
        uint64 endAddr;
    } memoryZones[256];
    uint64 memoryZonesCount = 0;

    struct multiboot_tag *tag;
    for (tag = (struct multiboot_tag*) (mboot2Addr + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag*) ((multiboot_uint8_t *) tag
                                        + ((tag->size + 7) & ~7))) {
        switch (tag->type) {
            case MULTIBOOT_TAG_TYPE_BASIC_MEMINFO: ;
                //struct multiboot_tag_basic_meminfo *mem = (struct multiboot_tag_basic_meminfo*) tag;
                //kprintf("Basic memory area %x - %x\n",
                //        mem->mem_lower,
                //        mem->mem_upper);
                break;
            case MULTIBOOT_TAG_TYPE_MMAP: ;
                kprintf("[GRUB] Memory map:\n");
                multiboot_memory_map_t *mmap;
                struct multiboot_tag_mmap *mmapTag = (struct multiboot_tag_mmap*) tag;

                for (mmap = mmapTag->entries;
                     (multiboot_uint8_t*) mmap < (multiboot_uint8_t*) tag + tag->size;
                     mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                            + mmapTag->entry_size)) {
                    kprintf("[addr %llx, len %llx] ",
                            mmap->addr,
                            mmap->len);

                    memoryEnd += mmap->len;

                    switch (mmap->type) {
                        case MULTIBOOT_MEMORY_AVAILABLE:
                            memoryZones[memoryZonesCount].startAddr = mmap->addr;
                            memoryZones[memoryZonesCount].endAddr = mmap->addr + mmap->len;
                            memoryZonesCount++;

                            kprintf("AVAILABLE\n");
                            break;
                        case MULTIBOOT_MEMORY_RESERVED:
                            kprintf("RESERVED\n");
                            break;
                        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                            kprintf("ACPI RECLAIMABLE\n");
                            break;
                        case MULTIBOOT_MEMORY_NVS:
                            kprintf("NVS\n");
                            break;
                        case MULTIBOOT_MEMORY_BADRAM:
                            kprintf("BAD RAM\n");
                    }
                }
                break;
            default:
                //kprintf("Unkown tag %x, size %x\n", tag->type, tag->size);
                break;
        }
    }

    // Inits Placement Address Allocator
    KERNEL_CHECK(PAA_init((size_t) &linker_kernelEnd));
    KERNEL_CHECK(PAA_alloc(sizeof(struct BitmapPMA) * memoryZonesCount, (void*) &g_PMAVM, 0x1000));

    g_PMAVM_num = memoryZonesCount;

    // Inits Physical Memory Manager
    KERNEL_CHECK(PMM_init(memoryZonesCount));
    for (size_t i = 0; i < memoryZonesCount; i++) {
        KERNEL_CHECK(BitmapPMA_createAllocator(&g_PMAVM[i], FRAME_SIZE,
                                  (size_t) memoryZones[i].startAddr,
                                  (size_t) memoryZones[i].endAddr));

        if (memoryZones[i].endAddr < 0x100000) {
            KERNEL_CHECK(PMM_addAllocator((void*) &g_PMAVM[i], PMM_FOR_DMA,
                    &BitmapPMA_alloc, &BitmapPMA_free, &BitmapPMA_reserve, &BitmapPMA_check));
        }
        else {
            KERNEL_CHECK(PMM_addAllocator((void*) &g_PMAVM[i], PMM_FOR_VIRTUAL_MEMORY,
                    &BitmapPMA_alloc, &BitmapPMA_free, &BitmapPMA_reserve, &BitmapPMA_check));
        }
    }
    kprintf("Memory size: %lld MiBs\n", memoryEnd / 1024 / 1024);

    kprintf("Memory end: %llx\n", memoryEnd);

    g_kernelArea.textStartAddr      = (size_t) &linker_textStart;
    g_kernelArea.textEndAddr        = (size_t) &linker_textEnd;
    g_kernelArea.rodataStartAddr    = (size_t) &linker_rodataStart;
    g_kernelArea.rodataEndAddr      = (size_t) &linker_rodataEnd;
    g_kernelArea.dataStartAddr      = (size_t) &linker_dataStart;
    g_kernelArea.dataEndAddr        = (size_t) &linker_dataEnd;
    g_kernelArea.bssStartAddr       = (size_t) &linker_bssStart;
    g_kernelArea.bssEndAddr         = (size_t) &linker_bssEnd;
    g_kernelArea.endPlacementAddr   = PAA_getCurrentAddress();

    KERNEL_CHECK(PMM_reserve(g_kernelArea.textStartAddr,
                g_kernelArea.endPlacementAddr - g_kernelArea.textStartAddr,
               PMM_FOR_VIRTUAL_MEMORY));

    KERNEL_CHECK(IA32_4KB_initKernelPaging(&g_kernelPaging));

    KERNEL_CHECK(IA32_4KB_switchDirectory(&g_kernelPaging));

    adjustPointers();

    return ERROR_SUCCESS;
}
