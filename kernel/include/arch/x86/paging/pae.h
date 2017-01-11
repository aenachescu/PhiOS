#ifndef PhiOs_arch_x86_paging_pae
#define PhiOs_arch_x86_paging_pae

#include "types.h"
#include "errors.h"

#define PAGING_PAE_PDPTE_NUMBER  4
#define PAGING_PAE_PDE_NUMBER    512
#define PAGING_PAE_PTE_NUMBER    512

/*
 * structs for page directory with page size 2MB
 */

// maps 2MB
struct PAE_PageDirectory_2MB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisabled  :  1;
    uint64 accessed       :  1;
    uint64 dirty          :  1;
    uint64 pageSize       :  1; // must be 1
    uint64 global         :  1;
    uint64 ignored        :  3;
    uint64 pat            :  1;
    uint64 alwaysZero1    :  8;
    uint64 address        : 31;
    uint64 alwaysZero2    : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 1GB
struct PAE_PageDirectory_2MB
{
    struct PAE_PageDirectory_2MB_Entry entries[PAGING_PAE_PDE_NUMBER];
} __attribute__((packed));

/*
 * structs for page directory with page size 4kb
 */

// maps 4kb
struct PAE_PageTable_4KB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisabled  :  1;
    uint64 accessed       :  1;
    uint64 dirty          :  1;
    uint64 pat            :  1; // reserved
    uint64 global         :  1;
    uint64 ignored        :  3;
    uint64 address        : 40;
    uint64 alwaysZero     : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 2mb
struct PAE_PageTable_4KB
{
    struct PAE_PageTable_4KB_Entry entries[PAGING_PAE_PTE_NUMBER];
} __attribute__((packed));

// maps 2mb
struct PAE_PageDirectory_4KB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisabled  :  1;
    uint64 accessed       :  1;
    uint64 ignored1       :  1;
    uint64 pageSize       :  1; // must be 0
    uint64 ignored2       :  4;
    uint64 address        : 40;
    uint64 alwaysZero     : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 1GB with pages of 4KB
struct PAE_PageDirectory_4KB
{
    struct PAE_PageDirectory_4KB_Entry entries[PAGING_PAE_PDE_NUMBER];
    struct PAE_PageTable_4KB *addresses[PAGING_PAE_PDE_NUMBER];
} __attribute__((packed));

/*
 * structs for PDPT. PDPT entry is independent of page size.
 */

// maps 1GB. It's same structure for both page sizes (4KB and 2MB).
struct PAE_PageDirectoryPointerTable_Entry
{
    uint64 present       :  1;
    uint64 alwaysZero1   :  2;
    uint64 writeThrough  :  1;
    uint64 cacheDisabled :  1;
    uint64 alwaysZero2   :  4;
    uint64 ignored       :  3;
    uint64 address       : 40;
    uint64 alwaysZero3   : 10;
} __attribute__((packed));

// maps 4GB with pages of 4KB
struct PAE_PageDirectoryPointerTable_4KB
{
    struct PAE_PageDirectoryPointerTable_Entry entries[PAGING_PAE_PDPTE_NUMBER];
    struct PAE_PageDirectory_4KB *addresses[PAGING_PAE_PDPTE_NUMBER];
} __attribute__((packed));

// maps 4GB with pages of 2MB
struct PAE_PageDirectoryPointerTable_2MB
{
    struct PAE_PageDirectoryPointerTable_Entry entries[PAGING_PAE_PDPTE_NUMBER];
    struct PAE_PageDirectory_2MB *addresses[PAGING_PAE_PDPTE_NUMBER];
} __attribute__((packed));

#endif
