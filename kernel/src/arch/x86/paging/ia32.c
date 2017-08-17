#include "kernel/include/arch/x86/paging/ia32.h"
#include "kernel/include/memory/pmm.h"
#include "cpu.h"

extern struct KernelArea g_kernelArea;

#define IA32_4KB_PD_VIRTUAL_ADDRESS \
    ((struct IA32_PageDirectory_4KB*) 0x003FF000) // 3MiB + 1020KiB

#define IA32_4KB_PT_VIRTUAL_ADDRESS \
    ((struct IA32_PageTable_4KB*) 0x00400000) // 4MiB

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
    size_t error = ERROR_SUCCESS;
    uint64 pdAddr = 0;
    struct IA32_PageDirectory_4KB *pd = NULL;

    do
    {
        error = PMM_alloc(
            &pdAddr,
            (uint64) sizeof(struct IA32_PageDirectory_4KB),
            (uint8) PMM_FOR_VIRTUAL_MEMORY
        );
        if (error != ERROR_SUCCESS)
        {
            a_paging->pagingStruct = NULL;
            break;
        }

        // TODO: optimize using kmemset()
        pd = (struct IA32_PageDirectory_4KB*) ((uint32)pdAddr);
        for (uint32 i = 0; i < PAGING_IA32_PDE_NUMBER; i++)
        {
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
    size_t error = ERROR_SUCCESS;
    uint64 pdAddr = (uint64) a_paging->pagingStruct;

    do
    {
        if (pdAddr != 0)
        {
            error = PMM_free(
                pdAddr,
                (uint64) sizeof(struct IA32_PageDirectory_4KB),
                (uint8) PMM_FOR_VIRTUAL_MEMORY
            );
            if (error != ERROR_SUCCESS)
            {
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
    if (pageId == NULL || tableId == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    uint32 tmp = virtualAddress / 4096;
    *pageId = tmp % 1024;
    *tableId = tmp / 1024;

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

uint32 IA32_4KB_initKernelPaging(
    struct Paging *a_paging)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t error = ERROR_SUCCESS;

    do
    {
        error = helper_IA32_4KB_createPaging(a_paging);
        if (error != ERROR_SUCCESS)
        {
            helper_IA32_4KB_deletePaging(a_paging);
            break;
        }

        uint32 pt0Addr = 0; // physical address for PT0
        uint32 pt1Addr = 0; // physical address for PT1
        uint32 pdAddr  = 0; // physical address for PD
        struct IA32_PageDirectory_4KB *pd = NULL;
        struct IA32_PageTable_4KB *pt0 = NULL;
        struct IA32_PageTable_4KB *pt1 = NULL;
        uint32 pageFlags = 0, pageTableFlags = 0;
        uint64 tmpAddr = 0;

        // alloc physical memory for 2 PTs
        error = PMM_alloc(
            &tmpAddr,
            (uint64) 2 * sizeof(struct IA32_PageTable_4KB),
            (uint8) PMM_FOR_VIRTUAL_MEMORY
        );
        if (error != ERROR_SUCCESS)
        {
            // cleanup...
            break;
        }

        pt0Addr = (uint32) tmpAddr;
        pt1Addr = pt0Addr + sizeof(struct IA32_PageTable_4KB);

        pdAddr = (uint32) a_paging->pagingStruct;

        pd = (struct IA32_PageDirectory_4KB*) a_paging->pagingStruct;
        pt0 = (struct IA32_PageTable_4KB*) pt0Addr;
        pt1 = (struct IA32_PageTable_4KB*) pt1Addr;

        // create flags for pages
        pageFlags |= IA32_4KB_PAGE_PRESENT;
        pageFlags |= IA32_4KB_PAGE_WRITE;
        pageFlags |= IA32_4KB_PAGE_WRITE_THROUGH;

        // create flags for page tables
        pageTableFlags |= IA32_4KB_PAGE_TABLE_PRESENT;
        pageTableFlags |= IA32_4KB_PAGE_TABLE_WRITE;
        pageTableFlags |= IA32_4KB_PAGE_TABLE_WRITE_THROUGH;

        helper_IA32_4KB_initPageTable(pt0);
        helper_IA32_4KB_initPageTable(pt1);

        // map the first page table
        pd->entries[0].data = (pageTableFlags | pt0Addr);

        // map the second page table
        pd->entries[1].data = (pageTableFlags | pt1Addr);

        // map the page where it's stored the first page table
        pt1->entries[0].data = (pageFlags | pt0Addr);

        // map the page where it's stored the second page table
        pt1->entries[1].data = (pageFlags | pt1Addr);

        // map the page where it's stored the PD
        pt0->entries[1023].data = (pageFlags | pdAddr);

        uint32 kernelEnd = g_kernelArea.endPlacementAddr;
        if (kernelEnd & 0x00000FFF)
        {
            kernelEnd &= 0xFFFFF000;
            kernelEnd += 0x00001000;
        }

        // map the page tables where it's stored the kernel
        uint32 pagesNum = kernelEnd - g_kernelArea.textStartAddr;
        pagesNum /= 4096;
        uint32 ptNum = pagesNum / PAGING_IA32_PTE_NUMBER;
        if (pagesNum % PAGING_IA32_PTE_NUMBER != 0)
            ptNum++;

        uint32 ptAddr = 0;
        PMM_alloc(
            &tmpAddr,
            (uint64) ptNum * sizeof(struct IA32_PageTable_4KB),
            (uint8) PMM_FOR_VIRTUAL_MEMORY
        );
        ptAddr = (uint32) tmpAddr;

        uint32 physicalAddr = g_kernelArea.textStartAddr;
        for (uint32 i = 0; i < ptNum; i++)
        {
            uint32 addr = ptAddr + i * sizeof(struct IA32_PageTable_4KB);
            struct IA32_PageTable_4KB *pt = (struct IA32_PageTable_4KB*) addr;
            helper_IA32_4KB_initPageTable(pt);

            pd->entries[768 + i].data = (pageTableFlags | addr);
            pt1->entries[768 + i].data = (pageFlags | addr);

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

uint32 IA32_4KB_init(
    struct Paging *a_paging, 
    struct Paging *a_currentPaging)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t error = ERROR_SUCCESS;

    error = helper_IA32_4KB_createPaging(a_paging);
    if (error != ERROR_SUCCESS)
    {
        return error;
    }

    struct IA32_4KB_Paging_AllocParam allocParam;
    struct AllocFuncParam request;
    size_t returnedAddress  = 0;
    size_t requestedAddress = 3221225472; // 3GB
    size_t difference       = 0;

    do
    {
        // init request
        request.pagingType = PAGING_TYPE_IA32_4KB;
        request.param      = &allocParam;

        // init allocParam for the kernel code area
        allocParam.flag             = PAGING_FLAG_ALLOC_SHARED_MEMORY   |
                                      PAGING_FLAG_ALLOC_AT_ADDRESS      |
                                      PAGING_FLAG_ALLOC_MAPS_KERNEL;
        allocParam.user             = false;
        allocParam.write            = false;
        allocParam.cacheDisabled    = false;
        allocParam.writeThrough     = true;
        allocParam.virtualAddress   = requestedAddress;
        allocParam.length           = g_kernelArea.textEndAddr -
                                      g_kernelArea.textStartAddr;
        allocParam.physicalAddress  = g_kernelArea.textStartAddr;

        error = IA32_4KB_alloc(a_paging, &request, &returnedAddress);
        if (error != ERROR_SUCCESS)
        {
            break;
        }

        requestedAddress = allocParam.length;
        if ((difference = allocParam.length % 4096) != 0)
        {
            requestedAddress += (4096 - difference);
        }

        allocParam.virtualAddress   = requestedAddress;
        allocParam.length           = g_kernelArea.rodataEndAddr -
                                      g_kernelArea.rodataStartAddr;
        allocParam.physicalAddress  = g_kernelArea.rodataStartAddr;

        error = IA32_4KB_alloc(a_paging, &request, &returnedAddress);
        if (error != ERROR_SUCCESS)
        {
            break;
        }

        requestedAddress = allocParam.length;
        if ((difference = allocParam.length % 4096) != 0)
        {
            requestedAddress += (4096 - difference);
        }

        allocParam.write            = true;
        allocParam.virtualAddress   = requestedAddress;
        allocParam.length           = g_kernelArea.dataEndAddr -
                                      g_kernelArea.dataStartAddr;
        allocParam.physicalAddress  = g_kernelArea.dataStartAddr;

        error = IA32_4KB_alloc(a_paging, &request, &returnedAddress);
        if (error != ERROR_SUCCESS)
        {
            break;
        }
    } while (false);

    if (error != ERROR_SUCCESS)
    {
        helper_IA32_4KB_deletePaging(a_paging);
    }

    return error;
}

uint32 IA32_4KB_alloc(
    struct Paging *a_paging,
    struct AllocFuncParam *a_request,
    size_t *a_address)
{
    if (a_paging == NULL || a_request == NULL || a_address == NULL ||
        a_paging->pagingStruct == NULL || a_request->param == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_request->pagingType != PAGING_TYPE_IA32_4KB ||
        a_paging->pagingType != PAGING_TYPE_IA32_4KB)
    {
        return ERROR_INVALID_PARAMETER;
    }

    size_t error = ERROR_SUCCESS;

    *a_address = NULL;

    struct IA32_4KB_Paging_AllocParam *request;
    request = (struct IA32_4KB_Paging_AllocParam*) a_request->param;

    do
    {
        // get physical address and set to request
        // call allocArea
    } while (false);

    return error;
}

uint32 IA32_4KB_free(
    struct Paging *a_paging,
    struct FreeFuncParam *a_request)
{
    if (a_paging == NULL || a_request == NULL ||
        a_paging->pagingStruct == NULL || a_request->param == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_request->pagingType != PAGING_TYPE_IA32_4KB ||
        a_paging->pagingType != PAGING_TYPE_IA32_4KB)
    {
        return ERROR_INVALID_PARAMETER;
    }

    size_t error = ERROR_SUCCESS;

    struct IA32_4KB_Paging_FreeParam  *request;
    request = (struct IA32_4KB_Paging_FreeParam*)  a_request->param;

    do
    {
    } while (false);

    return error;
}

uint32 IA32_4KB_switchDirectory(
    struct Paging *a_paging,
    struct IA32_PageDirectory_4KB *a_pageDirectory)
{
    if (a_paging == NULL || a_pageDirectory == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t addr = (size_t) a_pageDirectory;
    writeCR3(addr);

    return ERROR_SUCCESS;
}

uint32 IA32_4KB_enablePaging(
    struct Paging *a_paging)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    IA32_4KB_switchDirectory(a_paging, (struct IA32_PageDirectory_4KB*) a_paging->pagingStruct);
    size_t cr0 = readCR0();
    cr0 |= 0x80000001;
    writeCR0(cr0);

    return ERROR_SUCCESS;
}