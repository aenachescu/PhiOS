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
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 ignored1       :  1;
    uint32 pageSize       :  1; // must be 0
    uint32 ignored2       :  4;
    uint32 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint32 ignored3       : 11;
    uint32 executeDisable :  1;
} __attribute__((packed));

/*
 * structs for PML4 with page size 4KB
 */

// maps 4KB
struct IA32E_PageTable_4KB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 dirty          :  1;
    uint32 pat            :  1;
    uint32 global         :  1;
    uint32 ignored1       :  3;
    uint32 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint32 ignored2       :  7;
    uint32 protectionKey  :  4;
    uint32 executeDisable :  1;
} __attribute__((packed));

// maps 2MB
struct IA32E_PageTable_4KB
{
    struct IA32E_PageTable_4KB_Entry entries[PAGING_IA32E_PTE_NUMBER];
} __attribute__((packed));

// maps 2MB
struct IA32E_PageDirectory_4KB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 ignored1       :  1;
    uint32 pageSize       :  1; // must be 0
    uint32 ignored2       :  4;
    uint32 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint32 ignored3       : 11;
    uint32 executeDisable :  1;
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectory_4KB
{
    struct IA32E_PageDirectory_4KB_Entry entries[PAGING_IA32E_PDE_NUMBER];
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectoryPointerTable_4KB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 ignored1       :  1;
    uint32 pageSize       :  1; // must be 0
    uint32 ignored2       :  4;
    uint32 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint32 ignored3       : 11;
    uint32 executeDisable :  1;
} __attribute__((packed));

// maps 512GB
struct IA32E_PageDirectoryPointerTable_4KB
{
    struct IA32E_PageDirectoryPointerTable_4KB_Entry entries[PAGING_IA32E_PDPTE_NUMBER];
} __attribute__((packed));

// maps 256PB
struct IA32E_PageMapLevel4_4KB
{
    struct IA32E_PageMapLevel4_Entry entries[PAGING_IA32E_PML4E_NUMBER];
} __attribute__((packed));

/*
  * structs for PML4 with page size 2MB
  */

// maps 2MB
struct IA32E_PageDirectory_2MB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 dirty          :  1;
    uint32 pageSize       :  1; // must be 1
    uint32 global         :  1;
    uint32 ignored1       :  3;
    uint32 pat            :  1;
    uint32 alwaysZero     :  8;
    uint32 address        : 31; // M-1:21 is physical address, 51:M must be 0
    uint32 ignored2       :  7;
    uint32 protectionKey  :  4;
    uint32 executeDisable :  1;
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectory_2MB
{
    struct IA32E_PageDirectory_2MB_Entry entries[PAGING_IA32E_PDE_NUMBER];
} __attribute__((packed));

// maps 1GB
struct IA32E_PageDirectoryPointerTable_2MB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 ignored1       :  1;
    uint32 pageSize       :  1; // must be 0
    uint32 ignored2       :  4;
    uint32 address        : 40; // M-1:12 is physical address, 51:M must be 0
    uint32 ignored3       : 11;
    uint32 executeDisable :  1;
} __attribute__((packed));

// maps 512GB
struct IA32E_PageDirectoryPointerTable_2MB
{
    struct IA32E_PageDirectoryPointerTable_2MB_Entry entries[PAGING_IA32E_PDPTE_NUMBER];
} __attribute__((packed));

// maps 256PB
struct IA32E_PageMapLevel4_2MB
{
    struct IA32E_PageMapLevel4_Entry entries[PAGING_IA32E_PML4E_NUMBER];
} __attribute__((packed));

/*
 * structs for PML4 with page size 1GB
 */

// maps 1GB
struct IA32E_PageDirectoryPointerTable_1GB_Entry
{
    uint32 present        :  1;
    uint32 write          :  1;
    uint32 user           :  1;
    uint32 writeThrough   :  1;
    uint32 cacheDisable   :  1;
    uint32 accessed       :  1;
    uint32 dirty          :  1;
    uint32 pageSize       :  1; // must be 1
    uint32 global         :  1;
    uint32 ignored1       :  3;
    uint32 pat            :  1;
    uint32 alwaysZero     : 17;
    uint32 address        : 22; // M-1:30 must be address, 51:M must be 0
    uint32 ignored2       :  7;
    uint32 protectionKey  :  4;
    uint32 executeDisable :  1;
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
} __attribute__((packed));

#endif
