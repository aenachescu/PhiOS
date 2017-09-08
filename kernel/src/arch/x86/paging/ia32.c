#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/memory/pmm.h"
#include "kernel/include/arch/x86/registers.h"
#include "include/cpu.h"
#include "kernel/include/arch/x86/paging/paging.h"

extern struct KernelArea g_kernelArea;

#define IA32_4KB_PD_VIRTUAL_ADDRESS \
    ((struct IA32_PageDirectory_4KB*) 0xFFBFF000) // 3GiB + 1019MiB + 1020KiB

#define IA32_4KB_PT_VIRTUAL_ADDRESS \
    ((struct IA32_PageTable_4KB*) 0xFFC00000) // 3GiB + 1020MiB

#define KERNEL_VIRTUAL_ADDRESS ((uint32) 0xC0000000) // 3GiB

#define IA32_4KB_PAGE_PRESENT           ((uint32) 0x00000001)
#define IA32_4KB_PAGE_WRITE             ((uint32) 0x00000002)
#define IA32_4KB_PAGE_USER              ((uint32) 0x00000004)
#define IA32_4KB_PAGE_WRITE_THROUGH     ((uint32) 0x00000008)
#define IA32_4KB_PAGE_CACHE_DISABLED    ((uint32) 0x00000010)
#define IA32_4KB_PAGE_ACCESSED          ((uint32) 0x00000020)
#define IA32_4KB_PAGE_DIRTY             ((uint32) 0x00000040)
#define IA32_4KB_PAGE_PAT               ((uint32) 0x00000080)
#define IA32_4KB_PAGE_GLOBAL            ((uint32) 0x00000100)
#define IA32_4KB_PAGE_IGNORED           ((uint32) 0x00000E00)
#define IA32_4KB_PAGE_ADDRESS           ((uint32) 0xFFFFF000)

#define IA32_4KB_PAGE_TABLE_PRESENT         ((uint32) 0x00000001)
#define IA32_4KB_PAGE_TABLE_WRITE           ((uint32) 0x00000002)
#define IA32_4KB_PAGE_TABLE_USER            ((uint32) 0x00000004)
#define IA32_4KB_PAGE_TABLE_WRITE_THROUGH   ((uint32) 0x00000008)
#define IA32_4KB_PAGE_TABLE_CACHE_DISABLED  ((uint32) 0x00000010)
#define IA32_4KB_PAGE_TABLE_ACCESSED        ((uint32) 0x00000020)
#define IA32_4KB_PAGE_TABLE_IGNORED1        ((uint32) 0x00000040)
#define IA32_4KB_PAGE_TABLE_PAGE_SIZE       ((uint32) 0x00000080)
#define IA32_4KB_PAGE_TABLE_IGNORED2        ((uint32) 0x00000F00)
#define IA32_4KB_PAGE_TABLE_ADDRESS         ((uint32) 0xFFFFF000)

#define CHECK_ALIGN(addr, align) ((addr & (align - 1)) != 0)
#define ALIGN(addr, align) addr = addr & (~(align - 1))
#define IS_POWER_OF_2(x) (!(x & (x - 1)))

static uint32 helper_IA32_4KB_allocPageFlagsToIntelFlags(
    uint32 a_allocPageFlags)
{
    uint32 ret = 0;

    if (a_allocPageFlags & PAGING_ALLOC_PAGE_FLAG_WRITE) {
        ret |= IA32_4KB_PAGE_WRITE;
    }

    if (a_allocPageFlags & PAGING_ALLOC_PAGE_FLAG_USER) {
        ret |= IA32_4KB_PAGE_USER;
    }

    if (a_allocPageFlags & PAGING_ALLOC_PAGE_FLAG_WRITE_THROUGH) {
        ret |= IA32_4KB_PAGE_WRITE_THROUGH;
    }

    if (a_allocPageFlags & PAGING_ALLOC_PAGE_FLAG_CACHE_DISABLED) {
        ret |= IA32_4KB_PAGE_CACHE_DISABLED;
    }

    if (a_allocPageFlags & PAGING_ALLOC_PAGE_FLAG_GLOBAL) {
        ret |= IA32_4KB_PAGE_GLOBAL;
    }

    return ret;
}

static uint32 helper_IA32_4KB_createPaging(
    struct Paging *a_paging)
{
    uint32 error = ERROR_SUCCESS;
    uint64 pdAddr = 0;
    struct IA32_PageDirectory_4KB *pd = NULL;

    do {
        error = PMM_alloc(
            &pdAddr,
            (uint64) sizeof(struct IA32_PageDirectory_4KB),
            (uint8) PMM_FOR_VIRTUAL_MEMORY
        );
        if (error != ERROR_SUCCESS) {
            a_paging->pagingStruct = NULL;
            break;
        }

        // TODO: optimize using kmemset()
        pd = (struct IA32_PageDirectory_4KB*) ((uint32)pdAddr);
        for (uint32 i = 0; i < PAGING_IA32_PDE_NUMBER; i++) {
            pd->entries[i].data = 0;
        }

        a_paging->pagingType                = PAGING_TYPE_IA32_4KB;
        a_paging->locked                    = 0;
        a_paging->pagingStruct              = (void*) pd;
        a_paging->allocFn                   = IA32_4KB_alloc;
        a_paging->freeFn                    = IA32_4KB_free;

        // TODO: here we should calculate what's the actual available address space
        // we must remove from 4 GiB the reserved pages
        a_paging->freeVirtualMemory         = 0xFFFFFFFF;
        a_paging->lastAllocatedAddress      = 0x1000;

    } while (false);

    return error;
}

static uint32 helper_IA32_4KB_deletePaging(
    struct Paging *a_paging)
{
    uint32 error = ERROR_SUCCESS;
    uint64 pdAddr = (uint32) a_paging->pagingStruct;

    do {
        if (pdAddr != 0) {
            error = PMM_free(
                pdAddr,
                (uint64) sizeof(struct IA32_PageDirectory_4KB),
                (uint8) PMM_FOR_VIRTUAL_MEMORY
            );
            if (error != ERROR_SUCCESS) {
                break;
            }
        }

        a_paging->pagingType                = PAGING_TYPE_NONE;
        a_paging->locked                    = 0;
        a_paging->pagingStruct              = NULL;
        a_paging->allocFn                   = NULL;
        a_paging->freeFn                    = NULL;
        a_paging->freeVirtualMemory         = 0;
        a_paging->lastAllocatedAddress      = 0;

    } while (false);

    return error;
}

static uint32 helper_IA32_4KB_getPositionForVirtualAddress(
    uint32 a_virtualAddress,
    uint32 *a_pageId,
    uint32 *a_tableId)
{
    uint32 tmp = a_virtualAddress / 4096;

    if (a_pageId != NULL) {
        *a_pageId = tmp % PAGING_IA32_PTE_NUMBER;
    }

    if (a_tableId != NULL) {
        *a_tableId = tmp / PAGING_IA32_PTE_NUMBER;
    }

    return ERROR_SUCCESS;
}

static void helper_IA32_4KB_allocPageTable(
    struct IA32_PageDirectory_4KB *a_pageDirectory,
    uint32 a_pageTableId,
    uint32 a_pageTableFlags
)
{
    // set flags for the page table
    a_pageDirectory->entries[a_pageTableId].data = a_pageTableFlags;

    // alloc physical memory for the pages
    uint64 ptPhysAddr;
    PMM_alloc(
        &ptPhysAddr, 
        sizeof(struct IA32_PageTable_4KB_Entry) * PAGING_IA32_PTE_NUMBER,
        PMM_FOR_VIRTUAL_MEMORY
    );

    struct IA32_PageTable_4KB *pt0 = IA32_4KB_PT_VIRTUAL_ADDRESS;
    struct IA32_PageTable_4KB *pt1023 = pt0 + 1023;
            
    // map the page table physical address to the virtual address
    pt1023->entries[a_pageTableId].data = IA32_4KB_PAGE_PRESENT       |
                                          IA32_4KB_PAGE_WRITE         |
                                          IA32_4KB_PAGE_WRITE_THROUGH | 
                                          (uint32) ptPhysAddr;
}

static void helper_IA32_4KB_initPageTable(
    struct IA32_PageTable_4KB *a_pt)
{
    // TODO: optimizes this for using kmemset()
    for (uint32 i = 0; i < PAGING_IA32_PTE_NUMBER; i++) {
        a_pt->entries[i].data = 0;
    }
}

__attribute__ ((unused))
static void helper_IA32_4KB_allocArea(
    uint32 a_start,
    uint32 a_end,
    uint32 a_physicalAddress,
    uint32 a_pageFlags,
    uint32 a_pageTableFlags)
{
    struct IA32_PageDirectory_4KB *pd = IA32_4KB_PD_VIRTUAL_ADDRESS;
    struct IA32_PageTable_4KB *pageTable = IA32_4KB_PT_VIRTUAL_ADDRESS;

    // set present flag by default
    a_pageFlags |= IA32_4KB_PAGE_PRESENT;
    a_pageTableFlags |= IA32_4KB_PAGE_TABLE_PRESENT;

    uint32 pageId = 0, pageTableId = 0;
    helper_IA32_4KB_getPositionForVirtualAddress(a_start, &pageId, &pageTableId);
    pageTable += pageTableId;

    while (a_start < a_end) {
        if (pd->entries[pageTableId].data == 0) {
            helper_IA32_4KB_allocPageTable(pd, pageTableId, a_pageTableFlags);
            helper_IA32_4KB_initPageTable(pageTable);
        }

        pageTable->entries[pageId].data = (a_pageFlags | a_physicalAddress);
        a_physicalAddress += 4096;

        pageId++;
        if (pageId == PAGING_IA32_PTE_NUMBER) {
            pageId = 0;
            pageTable++;
            pageTableId++;
        }

        a_start += 4096;
    }
}

static uint32 helper_IA32_4KB_getPagesAndPTNum(
    uint32 a_start,
    uint32 a_end,
    uint32 *a_pagesNum,
    uint32 *a_ptNum)
{
    if (a_end <= a_start) {
        return ERROR_INVALID_PARAMETER;
    }

    a_start &= 0xFFFFF000;

    if ((a_end & 0x00000FFF) != 0) {
        a_end &= 0xFFFFF000;
        a_end += 4096;
    }

    uint32 pagesNumber = (a_end - a_start) / 4096;

    if (a_pagesNum != NULL) {
        *a_pagesNum = pagesNumber;
    }

    if (a_ptNum != NULL) {
        *a_ptNum = pagesNumber / PAGING_IA32_PTE_NUMBER;
        if (pagesNumber % PAGING_IA32_PTE_NUMBER != 0) {
            (*a_ptNum)++;
        }
    }

    return ERROR_SUCCESS;
}

uint32 IA32_4KB_initKernelPaging(
    struct Paging *a_paging)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    uint32 error = ERROR_SUCCESS;

    do
    {
        error = helper_IA32_4KB_createPaging(a_paging);
        if (error != ERROR_SUCCESS)
        {
            helper_IA32_4KB_deletePaging(a_paging);
            break;
        }

        uint32 pt0Addr = 0; // physical address for PT0
        uint32 pt1022Addr = 0; // physical address for PT1022
        uint32 pt1023Addr = 0; // physical address for PT1023
        uint32 pdAddr  = 0; // physical address for PD
        struct IA32_PageDirectory_4KB *pd = NULL;
        struct IA32_PageTable_4KB *pt0 = NULL; // used for indentify map
        struct IA32_PageTable_4KB *pt1022 = NULL; // used to map PD
        struct IA32_PageTable_4KB *pt1023 = NULL; // used to map the page tables
        uint32 pageFlags = 0, pageTableFlags = 0;
        uint64 tmpAddr = 0;

        // alloc physical memory for 3 PTs
        error = PMM_alloc(
            &tmpAddr,
            (uint64) 3 * sizeof(struct IA32_PageTable_4KB),
            (uint8) PMM_FOR_VIRTUAL_MEMORY
        );
        if (error != ERROR_SUCCESS)
        {
            helper_IA32_4KB_deletePaging(a_paging);
            break;
        }

        pt0Addr    = (uint32) tmpAddr;
        pt1022Addr = pt0Addr + sizeof(struct IA32_PageTable_4KB);
        pt1023Addr = pt0Addr + sizeof(struct IA32_PageTable_4KB);

        pdAddr = (uint32) a_paging->pagingStruct;

        pd = (struct IA32_PageDirectory_4KB*) a_paging->pagingStruct;
        pt0    = (struct IA32_PageTable_4KB*) pt0Addr;
        pt1022 = (struct IA32_PageTable_4KB*) pt1022Addr;
        pt1023 = (struct IA32_PageTable_4KB*) pt1023Addr;

        // create flags for pages
        pageFlags |= IA32_4KB_PAGE_PRESENT;
        pageFlags |= IA32_4KB_PAGE_WRITE;
        pageFlags |= IA32_4KB_PAGE_WRITE_THROUGH;

        // create flags for page tables
        pageTableFlags |= IA32_4KB_PAGE_TABLE_PRESENT;
        pageTableFlags |= IA32_4KB_PAGE_TABLE_WRITE;
        pageTableFlags |= IA32_4KB_PAGE_TABLE_WRITE_THROUGH;

        // init the page tables
        helper_IA32_4KB_initPageTable(pt0);
        helper_IA32_4KB_initPageTable(pt1022);
        helper_IA32_4KB_initPageTable(pt1023);

        // map the page tables
        pd->entries[0].data    = (pageTableFlags | pt0Addr);
        pd->entries[1022].data = (pageTableFlags | pt1022Addr);
        pd->entries[1023].data = (pageTableFlags | pt1023Addr);

        // map the pages where are stored the page tables
        pt1023->entries[0].data    = (pageFlags | pt0Addr);
        pt1023->entries[1022].data = (pageFlags | pt1022Addr);
        pt1023->entries[1023].data = (pageFlags | pt1023Addr);

        // map the page where it's stored the PD
        pt1022->entries[1023].data = (pageFlags | pdAddr);

        uint32 pagesNum = 0;
        uint32 ptNum = 0;
        helper_IA32_4KB_getPagesAndPTNum(
            g_kernelArea.textStartAddr,
            g_kernelArea.endPlacementAddr,
            &pagesNum,
            &ptNum
        );

        uint32 ptAddr = 0;
        PMM_alloc(
            &tmpAddr,
            (uint64) ptNum * sizeof(struct IA32_PageTable_4KB),
            (uint8) PMM_FOR_VIRTUAL_MEMORY
        );
        ptAddr = (uint32) tmpAddr;

        // map the page tables where it's stored the kernel
        uint32 physicalAddr = g_kernelArea.textStartAddr;
        for (uint32 i = 0; i < ptNum; i++)
        {
            uint32 addr = ptAddr + i * sizeof(struct IA32_PageTable_4KB);
            struct IA32_PageTable_4KB *pt = (struct IA32_PageTable_4KB*) addr;
            helper_IA32_4KB_initPageTable(pt);

            pd->entries[768 + i].data = (pageTableFlags | addr);
            pt1023->entries[768 + i].data = (pageFlags | addr);

            uint32 max = PAGING_IA32_PTE_NUMBER;
            if (pagesNum < max)
                max = pagesNum;

            // map the pages where it's stored the kernel
            for (uint32 page = 0; page < max; page++)
            {
                pt->entries[page].data = (pageFlags | physicalAddr);
                physicalAddr += 4096;
            }

            pagesNum -= max;
        }

        // map the page where it's stored the boot.s file
        // so, map the virtual address from 1mb to 1mb and 4kb
        pt0->entries[256].data = (pageFlags | 0x00100000);

        // map video address (0x000B8000)
        pt0->entries[184].data = (pageFlags | 0x000B8000);
    } while (false);

    return error;
}

uint32 IA32_4KB_alloc(
    struct Paging *a_paging,
    const struct VirtualAllocRequest *a_request,
    uint64 *a_address)
{
    if (a_paging == NULL ||
        a_request == NULL ||
        a_address == NULL ||
        a_paging->pagingStruct == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_paging->pagingType != PAGING_TYPE_IA32_4KB) {
        return ERROR_INVALID_FUNCTION;
    }

    if (a_request->length == 0 || a_request->length > 0xFFFFFFFF) {
        return ERROR_INVALID_SIZE;
    }

    uint32 error = ERROR_SUCCESS;
    *a_address = NULL;

    do {
        bool allocAtAddr = a_request->flags & PAGING_ALLOC_FLAG_AT_ADDRESS ? true : false;
        bool allocCloser = a_request->flags & PAGING_ALLOC_FLAG_CLOSER_TO_ADDRESS ? true : false;
        bool sharedMemory = a_request->flags & PAGING_ALLOC_FLAG_SHARED_MEMORY ? true : false;

        uint32 start = 0;
        uint32 end   = 0;
        
        uint32 physicalAddress = 0, pageTableFlags = 0;
        
        // craft page table falgs
        pageTableFlags = IA32_4KB_PAGE_TABLE_PRESENT | IA32_4KB_PAGE_TABLE_WRITE;
        pageTableFlags = a_request->pageFlags & IA32_4KB_PAGE_USER;

        // you can't alloc at some address or closer to it in the same time
        if (allocCloser && allocAtAddr) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }

        // check if physical memory can be used.
        if (sharedMemory) {
            // physical address must be in the first 4GiB
            if (a_request->physicalAddress > 0xFFFFFFFF) {
                error = ERROR_INVALID_PHYSICAL_ADDRESS;
                break;
            }

            // physical address must be multiple of 4KiB
            if (a_request->physicalAddress % 0x1000) {
                error = ERROR_INVALID_ALIGNMENT;
                break;
            }

            physicalAddress = a_request->physicalAddress;
        }

        if (allocAtAddr) {
            // in ia32 paging mode we have 4GiB of virtual memory
            if (a_request->virtualAddress > 0xFFFFFFFF) {
                error = ERROR_INVALID_VIRTUAL_ADDRESS;
                break;
            }

            start = (uint32) a_request->virtualAddress;
            end   = (uint32) a_request->length + start;

            if (start >= end) {
                error = ERROR_INVALID_VIRTUAL_ADDRESS;
                break;
            }

            // align the virtual addresses to 4 KiB
            ALIGN(start, 0x1000);

            if (CHECK_ALIGN(end, 0x1000)) {
                ALIGN(end, 0x1000);
                end += 0x1000;
            }

            // check if there is enough memory, but that does not guarantee that
            // we will find a continuous memory area of the required size
            // but when a large area is required, we can quickly say that we
            // don't have such a large area available without looking for
            // a continuous area.
            if (((uint64)(end - start)) > a_paging->freeVirtualMemory) {
                return ERROR_NO_FREE_VIRTUAL_MEMORY;
            }

            // if a user page was requested
            if (a_request->pageFlags & PAGING_ALLOC_PAGE_FLAG_USER) {
                // check if start and end addresses is in the user space area
                if (start < 0x1000 || start > 0xBFFFFFFF || end > 0xBFFFFFFF) {
                    error = ERROR_PERMISSION_DENIED;
                    break;
                }
            }

            // for each page from start address to end address check if it is free
            for (uint32 addr = start; addr < end; addr += 0x1000) {
                struct VirtualAddressInfo info;
                IA32_4KB_virtualQuery(
                    a_paging,
                    &info,
                    addr
                );
                    
                if (info.state != VIRTUAL_ADDRESS_STATE_FREE) {
                    error = ERROR_ALREADY_USED_AREA;
                    break;
                }
            }

            if (error != ERROR_SUCCESS) {
                break;
            }
        } else if (allocCloser) {
            /*// align the virtual address to 4 KiB
            ALIGN(a_request->virtualAddress, 0x1000);

            // align size too
            if (CHECK_ALIGN(a_request->length, 0x1000)) {
                ALIGN(a_request->length, 0x1000);
                a_request->length += 0x1000;
            }
            
            // check if there is enough memory
            size_t pagesToAlloc = a_request->length / 0x1000;
            if (pagesToAlloc > a_paging->freeVirtualMemory) {
                return ERROR_NO_FREE_MEMORY;
            }*/

            //struct VirtualAddressInfo info;

            /*
            TODO: this should be finished
            the algorithm logic is the following:
            - starting from current address we are going page by page
              up and down until we find the necessary amount of free pages

            - algorithm is unfinished


            uint32 upperLimit = 0xFFFFFFFF - request->virtualAddress;
            uint32 lowerLimit = request->virtualAddress - 0x1000;
            size_t upperFoundPages = 0;
            size_t lowerFoundPages = 0;

            for (uint32 distance = 0x0;
                 distance < upperLimit || distance < lowerLimit; 
                 distance += 0x1000) {

                if (distance < upperLimit) {
                    uint32 addr = distance + request->virtualAddress;
                    IA32_4KB_virtualQuery(
                        a_paging,
                        &info,
                        addr
                    );
                    if (info.state == VIRTUAL_ADDRESS_STATE_FREE) {
                        if (request->user) {
                            if (addr < 0x1000 || addr > 0xBFFFFFFF) {
                                continue;
                            }
                        }
                        request->virtualAddress = addr;
                        break;
                    }
                }

                if (distance < lowerLimit) {
                    uint32 addr = request->virtualAddress - distance;
                    IA32_4KB_virtualQuery(
                        a_paging,
                        &info,
                        addr
                    );
                    if (info.state == VIRTUAL_ADDRESS_STATE_FREE) {
                        if (request->user) {
                            if (addr < 0x1000 || addr > 0xBFFFFFFF) {
                                continue;
                            }
                        }
                        request->virtualAddress = addr;
                        break;
                    }
                }
            }

            if (0) {
                error = ERROR_NO_FREE_VIRTUAL_MEMORY;
                break;
            }

            kprintf("%x\n", request->virtualAddress);*/

        } else {
            // TODO: search from last allocated page to end of address space for
            // an empty page, if there aren't, then search from 0x0 to last allocated page

            struct VirtualAddressInfo info;
            for (uint32 addr = 0x0; addr < 0xFFFFFFFF; addr += 0x1000) {
                IA32_4KB_virtualQuery(
                    a_paging,
                    &info,
                    addr
                );

                if (info.state) {

                }
            }

        }

        // if the memory is not shared, we should alloc some physical memory
        if (!sharedMemory) {
            uint64 tempAddr;
            error = PMM_alloc(
                &tempAddr, 
                (uint64) (end - start),
                PMM_FOR_VIRTUAL_MEMORY
            );

            if (error != ERROR_SUCCESS) {
                error = ERROR_NO_FREE_PHYSICAL_MEMORY;
                break;
            }

            if (tempAddr > 0xFFFFFFFF) {
                error = ERROR_INVALID_PHYSICAL_ADDRESS;
                break;
            }
            
            physicalAddress = (uint32) tempAddr;

            // the entire physical memory area must be in the first 4GiB.
            tempAddr += (end - start);
            if (tempAddr > 0x100000000L) {
                error = PMM_free(
                    physicalAddress,
                    (uint64) (end - start),
                    PMM_FOR_VIRTUAL_MEMORY
                );

                if (error != ERROR_SUCCESS) {
                    // TODO: something like BSOD
                }

                error = ERROR_INVALID_PHYSICAL_ADDRESS;
                break;
            }
        } else {
            uint64 physicalAddressEnd = physicalAddress;
            physicalAddressEnd += (end - start);
            if (physicalAddressEnd > 0x100000000L) {
                error = ERROR_INVALID_PHYSICAL_ADDRESS;
                break;
            }
        }

        helper_IA32_4KB_allocArea(
            start,
            end,
            physicalAddress,
            helper_IA32_4KB_allocPageFlagsToIntelFlags(a_request->pageFlags),
            pageTableFlags
        );
    } while (false);

    return error;
}

uint32 IA32_4KB_free(
    struct Paging *a_paging,
    const struct VirtualFreeRequest *a_request)
{
    if (a_paging == NULL ||
        a_request == NULL ||
        a_paging->pagingStruct == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_paging->pagingType != PAGING_TYPE_IA32_4KB) {
        return ERROR_INVALID_PARAMETER;
    }

    uint32 error = ERROR_SUCCESS;

    do {
    } while (false);

    return error;
}

uint32 IA32_4KB_virtualQuery(
    struct Paging *a_paging,
    struct VirtualAddressInfo *a_vaInfo,
    uint32 a_address)
{
    /*
     * TODO:
     * 1. add support for STATE_RESERVED, STATE_PRIVATE & STATE_SHARED
     * 2. add support for FLAG_EXECUTE
     */

    /*
     * Notes:
     * 1. present == 0 && physicalAddr != 0 ==> memory in swap
     * 2. present == 0 && physicalAddr == 0 ==> memory reserved
     * 3. present == 1                      ==> memory used
     */

    if (a_paging == NULL || a_vaInfo == NULL) {
        return ERROR_NULL_POINTER;
    }

    // init a_vaInfo (default values)
    a_vaInfo->baseAddress   = NULL;
    a_vaInfo->pageSize      = 0;
    a_vaInfo->flags         = VIRTUAL_ADDRESS_FLAG_UNKNOWN;
    a_vaInfo->state         = VIRTUAL_ADDRESS_STATE_UNKNOWN;

    uint32 pageTableId = 0;
    uint32 pageId = 0;
    helper_IA32_4KB_getPositionForVirtualAddress(a_address, &pageId, &pageTableId);

    struct IA32_PageDirectory_4KB *pd = IA32_4KB_PD_VIRTUAL_ADDRESS;

#define SET_FLAG(x) (a_vaInfo->flags |= x)

    if (pd->entries[pageTableId].data != 0) {
        // page table is mapped
        if (pd->entries[pageTableId].pageSize == 0) {
            // it's a page of 4KiBs
            struct IA32_PageTable_4KB *pt = IA32_4KB_PT_VIRTUAL_ADDRESS;
            pt += pageTableId;

            if (pt->entries[pageId].data != 0) {
                // page is mapped
                a_vaInfo->baseAddress   = (void*)(a_address & 0xFFFFF000); // aligned at 4KiBs
                a_vaInfo->pageSize      = 4; // 4KiBs, because we are in IA32 4KiBs mode

                // cast to IA32 4KiBs page
                struct IA32_PageTable_4KB_Entry page;
                page.data = pt->entries[pageId].data;

                // set state
                if (page.present == 1) {
                    a_vaInfo->state = VIRTUAL_ADDRESS_STATE_USED;
                } else {
                    if (page.address != 0) {
                        a_vaInfo->state = VIRTUAL_ADDRESS_STATE_IN_SWAP;
                    } else {
                        a_vaInfo->state = VIRTUAL_ADDRESS_STATE_RESERVED;
                    }
                }

                // set flags
                page.write          != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_READ_WRITE)    :
                                           SET_FLAG(VIRTUAL_ADDRESS_FLAG_READ_ONLY);
                page.user           != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_USER_MODE)     : 0;
                page.global         != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_GLOBAL)        : 0;
                page.cacheDisabled  != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_NO_CACHE)      : 0;
                page.writeThrough   != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_WRITE_THROUGH) : 0;
                page.accessed       != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_ACCESSED)      : 0;
                page.dirty          != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_DIRTY)         : 0;
            } else {
                // page isn't mapped
                a_vaInfo->baseAddress   = (void*)(a_address & 0xFFFFF000); // aligned at 4KiBs
                a_vaInfo->pageSize      = 4; // 4KiBs, because we are in IA32 4KiBs mode
                a_vaInfo->state         = VIRTUAL_ADDRESS_STATE_FREE;
            }
        } else {
            // it's a large page (4MiBs in that case)
            a_vaInfo->baseAddress   = (void*)(a_address & 0xFFC00000); // aligned at 4MiBs
            a_vaInfo->pageSize      = 4096; // 4MiBs

            // cast to IA32 4MiBs page
            struct IA32_PageDirectory_4MB_Entry page;
            page.data = pd->entries[pageTableId].data;

            // set state
            if (page.present == 1) {
                a_vaInfo->state = VIRTUAL_ADDRESS_STATE_USED;
            } else {
                if (page.address != 0) {
                    a_vaInfo->state = VIRTUAL_ADDRESS_STATE_IN_SWAP;
                } else {
                    a_vaInfo->state = VIRTUAL_ADDRESS_STATE_RESERVED;
                }
            }

            // set flags
            page.write          != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_READ_WRITE)    :
                                       SET_FLAG(VIRTUAL_ADDRESS_FLAG_READ_ONLY);
            page.user           != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_USER_MODE)     : 0;
            page.global         != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_GLOBAL)        : 0;
            page.cacheDisabled  != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_NO_CACHE)      : 0;
            page.writeThrough   != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_WRITE_THROUGH) : 0;
            page.accessed       != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_ACCESSED)      : 0;
            page.dirty          != 0 ? SET_FLAG(VIRTUAL_ADDRESS_FLAG_DIRTY)         : 0;
        }
    } else {
        // page table isn't mapped
        a_vaInfo->baseAddress   = (void*)(a_address & 0xFFFFF000); // aligned at 4KiBs
        a_vaInfo->pageSize      = 4; // 4KibS, because we are in IA32 4KiBs mode
        a_vaInfo->state         = VIRTUAL_ADDRESS_STATE_FREE;
    }

#undef SET_FLAG

    return ERROR_SUCCESS;
}

uint32 IA32_4KB_switchDirectory(
    struct Paging *a_paging)
{
    if (a_paging == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 addr = (uint32) a_paging->pagingStruct;
    writeCR3(addr);

    return ERROR_SUCCESS;
}

uint32 IA32_4KB_enablePaging(
    struct Paging *a_paging)
{
    if (a_paging == NULL) {
        return ERROR_NULL_POINTER;
    }

    IA32_4KB_switchDirectory(a_paging);
    uint32 cr0 = readCR0();
    cr0 |= 0x80000001;
    writeCR0(cr0);

    return ERROR_SUCCESS;
}
