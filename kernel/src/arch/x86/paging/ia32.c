#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/memory/pmm.h"
#include "include/cpu.h"

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
        a_paging->freeMappedVirtualMemory   = 0;
        a_paging->freeVirtualMemory         = 0;
        a_paging->lastAllocatedPage         = 1;

    } while (false);

    return error;
}

static uint32 helper_IA32_4KB_deletePaging(
    struct Paging *a_paging)
{
    uint32 error = ERROR_SUCCESS;
    uint64 pdAddr = (uint64) a_paging->pagingStruct;

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
        a_paging->freeMappedVirtualMemory   = 0;
        a_paging->freeVirtualMemory         = 0;
        a_paging->lastAllocatedPage         = 0;

    } while (false);

    return error;
}

static void helper_IA32_4KB_allocArea(
    struct Paging *a_paging,
    struct IA32_4KB_Paging_AllocParam *a_request)
{
    struct IA32_PageDirectory_4KB *pd = IA32_4KB_PD_VIRTUAL_ADDRESS;
    struct IA32_PageTable_4KB *pageTable = IA32_4KB_PT_VIRTUAL_ADDRESS;

    size_t virtualAddress = a_request->virtualAddress & (~4095);
    size_t firstPageId = virtualAddress / 4096;
    size_t lastVirtualAddress = a_request->virtualAddress + a_request->length;
    if (lastVirtualAddress % 4096 != 0)
    {
        lastVirtualAddress &= (~4095);
        lastVirtualAddress += 4096;
    }

    size_t pageTableId = firstPageId / PAGING_IA32_PDE_NUMBER;
    size_t pageId = firstPageId % PAGING_IA32_PTE_NUMBER;

    size_t pagesNumber = (lastVirtualAddress - virtualAddress) / 4096;
    size_t physicalAddress = a_request->physicalAddress;

    uint32 data = 0;
    data |= (1 << 0); // set present
    a_request->write ? data |= (1 << 1) : 0; // set write
    a_request->user? data |= (1 << 2) : 0; // set user
    a_request->writeThrough ? data |= (1 << 3) : 0; // set writeThrough
    a_request->cacheDisabled ? data |= (1 << 4) : 0; // set cacheDisabled

    pageTable += pageTableId;

    while (pagesNumber != 0)
    {
        pagesNumber--;

        pageTable->entries[pageId].data = (data | physicalAddress);
        physicalAddress += 4096;

        pageId++;
        if (pageId == PAGING_IA32_PTE_NUMBER)
        {
            pageId = 0;
            pageTable++;
        }
    }

}

static uint32 helper_IA32_4KB_getPositionForVirtualAddress(
    uint32 virtualAddress,
    uint32 *pageId,
    uint32 *tableId)
{
    uint32 tmp = virtualAddress / 4096;

    if (pageId != NULL) {
        *pageId = tmp % PAGING_IA32_PTE_NUMBER;
    }

    if (tableId != NULL) {
        *tableId = tmp / PAGING_IA32_PTE_NUMBER;
    }

    return ERROR_SUCCESS;
}

static void helper_IA32_4KB_initPageTable(
    struct IA32_PageTable_4KB *pt)
{
    // TODO: optimizes this for using kmemset()
    for (uint32 i = 0; i < PAGING_IA32_PTE_NUMBER; i++)
    {
        pt->entries[i].data = 0;
    }
}

static uint32 helper_IA32_4KB_getPagesAndPTNum(
    uint32 start,
    uint32 end,
    uint32 *pagesNum,
    uint32 *ptNum)
{
    if (end <= start) {
        return ERROR_INVALID_PARAMETER;
    }

    start &= 0xFFFFF000;

    if ((end & 0x00000FFF) != 0) {
        end &= 0xFFFFF000;
        end += 4096;
    }

    uint32 pagesNumber = (end - start) / 4096;

    if (pagesNum != NULL) {
        *pagesNum = pagesNumber;
    }

    if (ptNum != NULL) {
        *ptNum = pagesNumber / PAGING_IA32_PTE_NUMBER;
        if (pagesNumber % PAGING_IA32_PTE_NUMBER != 0) {
            (*ptNum)++;
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
    struct AllocFuncParam *a_request,
    uint32 *a_address)
{
    if (a_paging == NULL
        || a_request == NULL
        || a_address == NULL
        || a_paging->pagingStruct == NULL
        || a_request->param == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_request->pagingType != PAGING_TYPE_IA32_4KB
        || a_paging->pagingType != PAGING_TYPE_IA32_4KB) {
        return ERROR_INVALID_PARAMETER;
    }

    uint32 error = ERROR_SUCCESS;

    *a_address = NULL;

    struct IA32_4KB_Paging_AllocParam *request;
    request = (struct IA32_4KB_Paging_AllocParam*) a_request->param;

    do {
        // get physical address and set to request
        // call allocArea
    } while (false);

    return error;
}

uint32 IA32_4KB_free(
    struct Paging *a_paging,
    struct FreeFuncParam *a_request)
{
    if (a_paging == NULL
        || a_request == NULL
        || a_paging->pagingStruct == NULL
        || a_request->param == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_request->pagingType != PAGING_TYPE_IA32_4KB
        || a_paging->pagingType != PAGING_TYPE_IA32_4KB) {
        return ERROR_INVALID_PARAMETER;
    }

    uint32 error = ERROR_SUCCESS;

    struct IA32_4KB_Paging_FreeParam *request;
    request = (struct IA32_4KB_Paging_FreeParam*)  a_request->param;

    do {
    } while (false);

    return error;
}

uint32 IA32_4KB_virtualQuery(
    struct Paging *a_paging,
    struct VirtualAddressInfo *a_vaInfo,
    uint32 a_address)
{
    if (a_paging == NULL || a_vaInfo == NULL) {
        return ERROR_NULL_POINTER;
    }

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
