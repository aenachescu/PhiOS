#ifndef PhiOS_paging32
#define PhiOS_paging32

#include "types.h"
#include "cpu.h"

// Number of pages contained by a page table on 32bit (PT)
#define PT32_PAGES_NUM 1024
// Number of page tables contained by a page directory on 32bit (PD)
#define PD32_PT_NUM 1024

// Page flags
#define PAGE_PRESENT 0x1
#define PAGE_READ_WRITE 0x2
#define PAGE_USER 0x4
#define PAGE_WRITE_THROUGH 0x8
#define PAGE_CACHE_DISABLE 0x16
#define PAGE_ACCESSED 0x32
#define PAGE_DIRTY 0x64
#define PAGE_GLOBAL 0x128

// Physical page
struct PageTableEntry32 {
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisabled  :  1;
    uint32 accessed       :  1;
    uint32 dirty          :  1;
    uint32 alwaysZero     :  1; // reserved
    uint32 global         :  1;
    uint32 ignored        :  3;
    uint32 address        : 20;
} __attribute__((packed));
typedef struct PageTableEntry32 PageTableEntry32;

// Physical page table
struct PageDirectoryEntry32 {
    uint32 present       :  1;
    uint32 write         :  1;
    uint32 user          :  1;
    uint32 writeThrough  :  1;
    uint32 cacheDisabled :  1;
    uint32 accessed      :  1;
    uint32 ignored1      :  1;
    uint32 PS            :  1; // If CR4.PSE = 1, must be 0 otherwise, this entry
                               // maps a 4-MByte page; otherwise, ignored
    uint32 ignored2      :  4;
    uint32 address       : 20;
} __attribute__((packed));
typedef struct PageDirectoryEntry32 PageDirectoryEntry32;

// Virtual page table
struct PageTable32 {
    PageTableEntry32 pages[PT32_PAGES_NUM];
} __attribute__((packed));
typedef struct PageTable32 PageTable32;

// Physical and virtual page directory
// When paging is enabled, in CR3 should be loaded address of physicalTables
struct PageDirectory32 {
    PageDirectoryEntry32 physicalTables[PD32_PT_NUM]; // Physical part of dir
    PageTable32 *virtualTables[PD32_PT_NUM]; // Virtual part of dir
};
typedef struct PageDirectory32 PageDirectory32;

size_t paging32_init();
size_t paging32_switchPageDirectory(PageDirectory32 *a_dir);
size_t paging32_map(uint32 a_virtual, uint32 a_physical, uint32 a_flags);
size_t paging32_unmap(uint32 a_virtual);
size_t paging32_mapArea(uint32 a_startVirtual, uint32 a_endVitual,
                        uint32 a_startPhysical, uint32 a_flags);
size_t paging32_unmapArea(uint32 a_startVirtual, uint32 a_endVitual);
size_t paging32_allocPage(uint32 a_virtual, uint32 a_flags);
size_t paging32_freePage(uint32 a_virtual);

#endif
