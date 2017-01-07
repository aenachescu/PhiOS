#ifndef PhiOS_arch_x86_paging_ia32e
#define PhiOS_arch_x86_paging_ia32e

#include "types.h"
#include "errors.h"

/*
 * struct PML4 entry. It's independent of page size.
 */
struct IA32E_PageMapLevel4_Entry
{};

/*
 * structs for PML4 with page size 4KB
 */

struct IA32E_PageTable_4KB_Entry
{};

struct IA32E_PageTable_4KB
{};

struct IA32E_PageDirectory_4KB_Entry
{};

struct IA32E_PageDirectory_4KB
{};

struct IA32E_PageDirectoryPointerTable_4KB_Entry
{};

struct IA32E_PageDirectoryPointerTable_4KB
{};

struct IA32E_PageMapLevel4_4KB
{};

/*
  * structs for PML4 with page size 2MB
  */

struct IA32E_PageDirectory_2MB_Entry
{};

struct IA32E_PageDirectory_2MB
{};

struct IA32E_PageDirectoryPointerTable_2MB_Entry
{};

struct IA32E_PageDirectoryPointerTable_2MB
{};

struct IA32E_PageMapLevel4_2MB
{};

/*
 * structs for PML4 with page size 1GB
 */

struct IA32E_PageDirectoryPointerTable_1GB_Entry
{};

struct IA32E_PageDirectoryPointerTable_1GB
{};

struct IA32E_PageMapLevel4_1GB
{};

#endif
