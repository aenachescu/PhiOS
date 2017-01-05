#ifndef PhiOs_arch_x86_paging_pae
#define PhiOs_arch_x86_paging_pae

#include "types.h"
#include "errors.h"

#define PAGING_PAE_PDPT_ENTRIES_NUMBER 4
#define PAGING_PAE_PAGE_DIRECTORY_ENTRIES_NUMBER 512
#define PAGING_PAE_PAGE_TABLE_ENTRIES_NUMBER 512

// maps 1gb
struct PAE_PageDirectoryPointerTable_Entry
{
    uint32 present       :  1;
    uint32 alwaysZero1   :  2;
    uint32 writeThrough  :  1;
    uint32 cacheDisabled :  1;
    uint32 alwaysZero2   :  4;
    uint32 ignored       :  3;
    uint32 address       : 40;
    uint32 alwaysZero3   : 10;
};

// maps 2mb
struct PAE_PageDirectory2MB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisabled  :  1;
    uint32 accessed       :  1;
    uint32 dirty          :  1;
    uint32 pageSize       :  1; // must be 1
    uint32 global         :  1;
    uint32 ignored        :  3;
    uint32 pat            :  1;
    uint32 alwaysZero1    :  8;
    uint32 address        : 31;
    uint32 alwaysZero2    : 11;
    uint32 executeDisable :  1;
};

// maps 1gb
struct PAE_PageDirectory2MB
{
    struct PAE_PageDirectory2MB_Entry entries[PAGING_PAE_PAGE_DIRECTORY_ENTRIES_NUMBER];
};

// maps 4kb
struct PAE_PageTable4KB_Entry
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
    uint32 address        : 40;
    uint32 alwaysZero     : 11;
    uint32 executeDisable :  1;
};

// maps 2mb
struct PAE_PageTable4KB
{
    struct PAE_PageTable4KB_Entry entries[PAGING_PAE_PAGE_TABLE_ENTRIES_NUMBER];
};

// maps 2mb
struct PAE_PageDirectory4KB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisabled  :  1;
    uint32 accessed       :  1;
    uint32 ignored1       :  1;
    uint32 pageSize       :  1; // must be 0
    uint32 ignored2       :  4;
    uint32 address        : 40;
    uint32 alwaysZero     : 11;
    uint32 executeDisable :  1;
};

// maps 1gb
struct PAE_PageDirectory4KB
{
    struct PAE_PageDirectory4KB_Entry entries[PAGING_PAE_PAGE_DIRECTORY_ENTRIES_NUMBER];
};

// maps 4gb
struct PAE_PageDirectoryPointerTable
{
    struct PAE_PageDirectoryPointerTable_Entry entries[PAGING_PAE_PDPT_ENTRIES_NUMBER];
};

#endif
