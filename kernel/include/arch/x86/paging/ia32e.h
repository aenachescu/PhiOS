#ifndef PhiOS_arch_x86_paging_ia32e
#define PhiOS_arch_x86_paging_ia32e

#include "types.h"
#include "errors.h"

#define PAGING_IA32E_PTE_NUMBER     512
#define PAGING_IA32E_PDE_NUMBER     512
#define PAGING_IA32E_PDPTE_NUMBER   512
#define PAGING_IA32E_PML4E_NUMBER   512

/*
 * struct PML4 entry. It's independent of page size.
 */
struct IA32E_PageMapLevel4_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 ignored1       :  1;
    uint64 pageSize       :  1; // must be 0
    uint64 ignored2       :  4;
    uint64 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint64 ignored3       : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

/*
 * structs for PML4 with page size 4KB
 */

// maps 4KB
struct IA32E_PageTable_4KB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 dirty          :  1;
    uint64 pat            :  1;
    uint64 global         :  1;
    uint64 ignored1       :  3;
    uint64 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint64 ignored2       :  7;
    uint64 protectionKey  :  4;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 2MB
struct IA32E_PageTable_4KB
{
    struct IA32E_PageTable_4KB_Entry entries[PAGING_IA32E_PTE_NUMBER];
} __attribute__((packed));

// maps 2MB
struct IA32E_PageDirectory_4KB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 ignored1       :  1;
    uint64 pageSize       :  1; // must be 0
    uint64 ignored2       :  4;
    uint64 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint64 ignored3       : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectory_4KB
{
    struct IA32E_PageDirectory_4KB_Entry entries[PAGING_IA32E_PDE_NUMBER];
    struct IA32E_PageTable_4KB *addresses[PAGING_IA32E_PDE_NUMBER];
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectoryPointerTable_4KB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 ignored1       :  1;
    uint64 pageSize       :  1; // must be 0
    uint64 ignored2       :  4;
    uint64 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint64 ignored3       : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 512GB
struct IA32E_PageDirectoryPointerTable_4KB
{
    struct IA32E_PageDirectoryPointerTable_4KB_Entry entries[PAGING_IA32E_PDPTE_NUMBER];
    struct IA32E_PageDirectory_4KB *addresses[PAGING_IA32E_PDPTE_NUMBER];
} __attribute__((packed));

// maps 256PB
struct IA32E_PageMapLevel4_4KB
{
    struct IA32E_PageMapLevel4_Entry entries[PAGING_IA32E_PML4E_NUMBER];
    struct IA32E_PageDirectoryPointerTable_4KB *addresses[PAGING_IA32E_PML4E_NUMBER];
} __attribute__((packed));

/*
  * structs for PML4 with page size 2MB
  */

// maps 2MB
struct IA32E_PageDirectory_2MB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 dirty          :  1;
    uint64 pageSize       :  1; // must be 1
    uint64 global         :  1;
    uint64 ignored1       :  3;
    uint64 pat            :  1;
    uint64 alwaysZero     :  8;
    uint64 address        : 31; // M-1:21 is physical address, 51:M must be 0
    uint64 ignored2       :  7;
    uint64 protectionKey  :  4;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectory_2MB
{
    struct IA32E_PageDirectory_2MB_Entry entries[PAGING_IA32E_PDE_NUMBER];
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectoryPointerTable_2MB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 ignored1       :  1;
    uint64 pageSize       :  1; // must be 0
    uint64 ignored2       :  4;
    uint64 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint64 ignored3       : 11;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 512GB
struct IA32E_PageDirectoryPointerTable_2MB
{
    struct IA32E_PageDirectoryPointerTable_2MB_Entry entries[PAGING_IA32E_PDPTE_NUMBER];
    struct IA32E_PageDirectory_2MB *addresses[PAGING_IA32E_PDPTE_NUMBER];
} __attribute__((packed));

// maps 256PB
struct IA32E_PageMapLevel4_2MB
{
    struct IA32E_PageMapLevel4_Entry entries[PAGING_IA32E_PML4E_NUMBER];
    struct IA32E_PageDirectoryPointerTable_2MB *addresses[PAGING_IA32E_PML4E_NUMBER];
} __attribute__((packed));

/*
 * structs for PML4 with page size 1GB
 */

// maps 1GB
struct IA32E_PageDirectoryPointerTable_1GB_Entry
{
    uint64 present        :  1;
    uint64 write          :  1;
    uint64 user           :  1;
    uint64 writeThrough   :  1;
    uint64 cacheDisable   :  1;
    uint64 accessed       :  1;
    uint64 dirty          :  1;
    uint64 pageSize       :  1; // must be 1
    uint64 global         :  1;
    uint64 ignored1       :  3;
    uint64 pat            :  1;
    uint64 alwaysZero     : 17;
    uint64 address        : 22; // M-1:30 must be address, 51:M must be 0
    uint64 ignored2       :  7;
    uint64 protectionKey  :  4;
    uint64 executeDisable :  1;
} __attribute__((packed));

// maps 512GB
struct IA32E_PageDirectoryPointerTable_1GB
{
    struct IA32E_PageDirectoryPointerTable_1GB_Entry entries[PAGING_IA32E_PDPTE_NUMBER];
} __attribute__((packed));

// maps 256PB
struct IA32E_PageMapLevel4_1GB
{
    struct IA32E_PageMapLevel4_Entry entries[PAGING_IA32E_PML4E_NUMBER];
    struct IA32E_PageDirectoryPointerTable_1GB *addresses[PAGING_IA32E_PML4E_NUMBER];
} __attribute__((packed));

#endif
