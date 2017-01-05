#ifndef PhiOS_arch_x86_paging_ia32
#define PhiOS_arch_x86_paging_ia32

#include "types.h"
#include "errors.h"

#define PAGING_IA32_PAGE_TABLE_ENTRIES_NUMBER       1024
#define PAGING_IA32_PAGE_DIRECTORY_ENTRIES_NUMBER   1024

// maps 4kb of virtual memory
struct PageTable4KB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisabled  :  1;
    uint32 accessed       :  1;
    uint32 dirty          :  1;
    uint32 pat            :  1; // reserved
    uint32 global         :  1;
    uint32 ignored        :  3;
    uint32 address        : 20;
};

// maps 4mb of virtual memory
struct PageTable4KB
{
    struct PageTable4KB_Entry entries[PAGING_IA32_PAGE_TABLE_ENTRIES_NUMBER];
};

// maps 4mb of virtual memory
struct PageDirectory4KB_Entry
{
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
};

// maps 4gb of virtual memory
struct PageDirectory_4KB
{
    struct PageDirectory4KB_Entry entries[PAGING_IA32_PAGE_DIRECTORY_ENTRIES_NUMBER];
};

struct PageDirectory_4MB
{

};

#endif
