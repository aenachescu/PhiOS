#include "kernel/include/arch/x86/init32.h"
#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/arch/x86/paging/paging.h"

#include "kernel/include/memory/paa.h"
#include "kernel/include/memory/pmm.h"
#include "kernel/include/memory/bitmap_pma.h"
#include "kernel/include/logging.h"

#include "drivers/video/include/vga/text_mode.h"
#include "drivers/acpi/include/acpi.h"
#include "drivers/acpi/include/acpi_xsdt.h"
#include "drivers/acpi/include/acpi_rsdt.h"
#include "drivers/serial/include/serial.h"

#include "kernel/include/multiboot2.h"

#include "include/cpu.h"

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
                KLOG_INFO("Found SMBios32 at [%p]", p);
                KLOG_INFO("SMBios32 info: major[%hhu], minor[%hhu], revision[%hhu]",
                    p[6], p[7], p[30]);
                break;
            }
        }
        p += 16;
    }

    if (p >= (uint8*) 0x100000) {
        KLOG_INFO("NOT found SMBios32");
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

    PMM_adjustPointers(offset);
    logging_adjustPointers(offset);

    for (size_t i = 0; i < g_PMAVM_num; i++) {
        g_PMAVM[i].bitmap = (size_t*) (((size_t)g_PMAVM[i].bitmap) + offset);
    }

    adjust_got(offset);
}

void ParseAcpi()
{
    RSDP2 rsdp2;
    uint32 acpiVer = ACPI_VERSION_UNKNOWN;
    acpi_getRSDP(&rsdp2, &acpiVer);

    if (acpiVer != ACPI_VERSION_2) {
        KLOG_WARNING("unsupported acpi version");
        return;
    }

    KLOG_INFO("revision: %hhu", rsdp2.rsdp.revision);
    KLOG_INFO("RSDT: %#0x, XSDT %#0llx", rsdp2.rsdp.rsdtAddr, rsdp2.xsdtAddr);

    XSDT xsdt;
    RSDT rsdt;
    uint32 err = 0;
    uint64 sratAddr64 = 0;
    uint32 sratAddr = 0;

    if (rsdp2.xsdtAddr == 0) {
        KLOG_INFO("RSDT address - %p", rsdp2.rsdp.rsdtAddr);
        err = acpi_rsdt_init(&rsdt, (uint8*) rsdp2.rsdp.rsdtAddr);
        if (err != ERROR_SUCCESS) {
            KLOG_WARNING("rsdt init failed - %u", err);
            return;
        }

        err = acpi_rsdt_findHeader(&rsdt, "SRAT", &sratAddr);
        if (err != ERROR_SUCCESS) {
            KLOG_WARNING("srat not found - %u", err);
            return;
        }

        KLOG_INFO("srat found at [%p]", (void*) sratAddr);
        return;
    }

    if (rsdp2.xsdtAddr > 0xFFFFFFFFULL) {
        KLOG_ERROR("incompatible addr");
        return;
    }

    err = acpi_xsdt_init(&xsdt, (uint8*) ((size_t) rsdp2.xsdtAddr));
    if (err != ERROR_SUCCESS) {
        KLOG_WARNING("xsdt init failed - %u", err);
        return;
    }

    err = acpi_xsdt_check32BitsCompatibility(&xsdt);
    if (err != ERROR_SUCCESS) {
        KLOG_WARNING("xsdt is incompatible - %u", err);
        return;
    }

    err = acpi_xsdt_findHeader(&xsdt, "SRAT", &sratAddr64);
    if (err != ERROR_SUCCESS) {
        KLOG_WARNING("not found SRAT in XSDT - %u", err);
        return;
    }

    KLOG_INFO("found SRAT in XSDT at %llu", sratAddr64);
}

uint32 init_init32(
    uint32 mboot2Magic,
    uint32 mboot2Addr)
{
    // Inits VGA
    VGA_Init();
    serial_init();

    logging_init();
    logging_addPfn(VGA_WriteString);
    logging_addPfn(serial_writeStringDefault);

    KLOG_INFO("PhiOS v0.0.1 32-bit");

    if (mboot2Magic != MULTIBOOT2_BOOTLOADER_MAGIC) {
        KLOG_FATAL("GRUB not multiboo2");
        return ERROR_NOT_FOUND;
    }

    if (mboot2Addr & 7) {
        KLOG_FATAL("Multiboot2 structure is not aligned");
        return ERROR_UNALIGNED_ADDRESS;
    }

    KLOG_INFO("GRUB multiboot2");

    detectSMBios();

    // Iterate over tags and collect info
    uint64 availableMemory = 0, totalMemory = 0;
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
            case MULTIBOOT_TAG_TYPE_MMAP:
                KLOG_INFO("GRUB memory map:");
                multiboot_memory_map_t *mmap;
                struct multiboot_tag_mmap *mmapTag = (struct multiboot_tag_mmap*) tag;

                for (mmap = mmapTag->entries;
                     (multiboot_uint8_t*) mmap < (multiboot_uint8_t*) tag + tag->size;
                     mmap = (multiboot_memory_map_t *) ((unsigned long) mmap
                            + mmapTag->entry_size)) {
                    totalMemory += mmap->len;

                    const char *state = "UNKNOWN";
                    switch (mmap->type) {
                        case MULTIBOOT_MEMORY_AVAILABLE:
                            memoryZones[memoryZonesCount].startAddr = mmap->addr;
                            memoryZones[memoryZonesCount].endAddr = mmap->addr + mmap->len;
                            memoryZonesCount++;
                            availableMemory += mmap->len;

                            state = "AVAILABLE";
                            break;
                        case MULTIBOOT_MEMORY_RESERVED:
                            state = "RESERVED";
                            break;
                        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
                            state = "ACPI RECLAIMABLE";
                            break;
                        case MULTIBOOT_MEMORY_NVS:
                            state = "NVS";
                            break;
                        case MULTIBOOT_MEMORY_BADRAM:
                            state = "BAD RAM";
                    }

                    KLOG("addr[%#0llx] len[%#0llx] %s", mmap->addr, mmap->len, state);
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

    KLOG_INFO("TotalMem: %llu MiBs, AvailableMem: %llu MiBs",
        totalMemory / 1024 / 1024, availableMemory / 1024 / 1024);

    g_kernelArea.textStartAddr      = (size_t) &linker_textStart;
    g_kernelArea.textEndAddr        = (size_t) &linker_textEnd;
    g_kernelArea.rodataStartAddr    = (size_t) &linker_rodataStart;
    g_kernelArea.rodataEndAddr      = (size_t) &linker_rodataEnd;
    g_kernelArea.dataStartAddr      = (size_t) &linker_dataStart;
    g_kernelArea.dataEndAddr        = (size_t) &linker_dataEnd;
    g_kernelArea.bssStartAddr       = (size_t) &linker_bssStart;
    g_kernelArea.bssEndAddr         = (size_t) &linker_bssEnd;
    g_kernelArea.endPlacementAddr   = PAA_getCurrentAddress();

    ParseAcpi();

    KERNEL_CHECK(PMM_reserve(g_kernelArea.textStartAddr,
                g_kernelArea.endPlacementAddr - g_kernelArea.textStartAddr,
               PMM_FOR_VIRTUAL_MEMORY));

    KERNEL_CHECK(IA32_4KB_initKernelPaging(&g_kernelPaging));

    KERNEL_CHECK(IA32_4KB_switchDirectory(&g_kernelPaging));

    adjustPointers();

    return ERROR_SUCCESS;
}
