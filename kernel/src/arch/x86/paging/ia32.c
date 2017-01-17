#include "types.h"
#include "errors.h"
#include "kstring.h"
#include "arch/x86/paging/ia32.h"
#include "arch/x86/paging/paging.h"
#include "memory/pmm.h"
#include "memory/paa.h"

#include "kstdio.h"

size_t IA32_4KB_initKernelStruct(struct Paging *a_paging,
                                 size_t a_codeStartAddr,
                                 size_t a_codeEndAddr,
                                 size_t a_rodataStartAddr,
                                 size_t a_rodataEndAddr,
                                 size_t a_rwdataStartAddr,
                                 size_t a_rwdataEndAddr)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_codeEndAddr   <= a_codeStartAddr    ||
        a_rodataEndAddr <  a_rodataStartAddr  ||
        a_rwdataEndAddr <  a_rwdataStartAddr)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if ((a_codeStartAddr   & 4095) != 0 ||
        (a_rodataStartAddr & 4095) != 0 ||
        (a_rwdataStartAddr & 4095) != 0)
    {
        return ERROR_UNALIGNED_ADDRESS;
    }

    size_t error = ERROR_SUCCESS;

    do
    {
    } while (false);

    return error;
}

size_t IA32_4KB_init(struct Paging *a_kernelPaging,
                     struct Paging *a_newPaging)
{
    if (a_kernelPaging == NULL || a_newPaging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    size_t error = ERROR_SUCCESS;

    do
    {
    } while (false);

    return error;
}

size_t IA32_4KB_alloc(struct Paging *a_paging,
                      struct AllocFuncParam *a_request,
                      size_t *a_address)
{
    if (a_paging == NULL || a_request == NULL || a_address == NULL ||
        a_paging->pagingStruct == NULL || a_request->param == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_request->pagingType != PAGING_TYPE_IA32_4KB)
    {
        return ERROR_INVALID_PARAMETER;
    }

    size_t error = ERROR_SUCCESS;

    *a_address = NULL;

    struct IA32_PageDirectory_4KB     *pd;
    struct IA32_4KB_Paging_AllocParam *request;

    pd      = (struct IA32_PageDirectory_4KB*)     a_paging->pagingStruct;
    request = (struct IA32_4KB_Paging_AllocParam*) a_request->param;

    do
    {
    } while (false);

    return error;
}

size_t IA32_4KB_free(struct Paging *a_paging,
                     struct FreeFuncParam *a_request)
{
    if (a_paging == NULL || a_request == NULL ||
        a_paging->pagingStruct == NULL || a_request->param == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_request->pagingType != PAGING_TYPE_IA32_4KB)
    {
        return ERROR_INVALID_PARAMETER;
    }

    size_t error = ERROR_SUCCESS;

    struct IA32_PageDirectory_4KB     *pd;
    struct IA32_4KB_Paging_FreeParam  *request;

    pd      = (struct IA32_PageDirectory_4KB*)     a_paging->pagingStruct;
    request = (struct IA32_4KB_Paging_FreeParam*)  a_request->param;

    do
    {
    } while (false);

    return error;
}

/*
struct Paging g_kernelPaging;
struct IA32_Paging_4KB g_kernelPagingStruct;
struct IA32_PageDirectory_4KB g_kernelPageDirectory;

void helper_IA32_4KB_allocPage(struct IA32_PageDirectory_4KB *currentPageDirectory,
                               size_t a_address,
                               uint32 a_flags,
                               bool   a_write,
                               bool   a_user,
                               bool   a_writeThrough,
                               bool   a_cacheDisabled)
{
    size_t pageNum = (a_address / (4 * KiB)) % PAGING_IA32_PTE_NUMBER;
    size_t tableNum = (a_address / (4 * KiB)) / PAGING_IA32_PDE_NUMBER;
    kprintf("MAP PAGE %x [%d][%d]\n", a_address, tableNum, pageNum);

    if (currentPageDirectory->addresses[tableNum] == NULL)
    {
        //kprintf("ALLOC NEW TABLE\n");
        size_t addrToAlloc;
        PAA_alloc(4 * KiB, &addrToAlloc, 4 * KiB);
        // A heap should be used to alloc new page tables...
        //PMM_alloc(&addrToAlloc, 1, PMM_FOR_VIRTUAL_MEMORY);

        currentPageDirectory->addresses[tableNum] = (struct IA32_PageTable_4KB*) addrToAlloc;
        currentPageDirectory->entries[tableNum].address = (uint32) addrToAlloc;
        currentPageDirectory->entries[tableNum].write = a_write;
        currentPageDirectory->entries[tableNum].user = a_user;
        currentPageDirectory->entries[tableNum].writeThrough = a_writeThrough;
        currentPageDirectory->entries[tableNum].cacheDisabled = a_cacheDisabled;
    }

    currentPageDirectory->addresses[tableNum]->entries[pageNum].address = (uint32) a_address;
    currentPageDirectory->addresses[tableNum]->entries[pageNum].present = true;
    currentPageDirectory->addresses[tableNum]->entries[pageNum].write = a_write;
    currentPageDirectory->addresses[tableNum]->entries[pageNum].user = a_user;
    currentPageDirectory->addresses[tableNum]->entries[pageNum].writeThrough = a_writeThrough;
    currentPageDirectory->addresses[tableNum]->entries[pageNum].cacheDisabled = a_cacheDisabled;
}

void helper_IA32_4KB_freePage(struct Paging *a_paging,
                              size_t a_address)
{
    size_t pageNum = (a_address / (4 * KiB)) % PAGING_IA32_PTE_NUMBER;
    size_t tableNum = (a_address / (4 * KiB)) / PAGING_IA32_PDE_NUMBER;

    struct IA32_Paging_4KB *currentPagingStruct = (struct IA32_Paging_4KB*) a_paging->pagingStruct;
    struct IA32_PageDirectory_4KB *currentPageDirectory = currentPagingStruct->currentPageDirectory;

    if (currentPageDirectory->addresses[tableNum] == NULL)
    {
        return ;
    }

    currentPageDirectory->addresses[tableNum]->entries[pageNum].present = false;
    asm volatile ("invlpg (%0)" : : "a" ((uint32) a_address));
}

size_t IA32_4KB_initKernelStruct(struct Paging *a_paging,
                                 size_t a_codeStartAddr,
                                 size_t a_codeEndAddr,
                                 size_t a_rodataStartAddr,
                                 size_t a_rodataEndAddr,
                                 size_t a_rwdataStartAddr,
                                 size_t a_rwdataEndAddr)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (a_codeEndAddr   <= a_codeStartAddr    ||
        a_rodataEndAddr <  a_rodataStartAddr  ||
        a_rwdataEndAddr <  a_rwdataStartAddr)
    {
        return ERROR_INVALID_PARAMETER;
    }

    if ((a_codeStartAddr   & 4095) != 0 ||
        (a_rodataStartAddr & 4095) != 0 ||
        (a_rwdataStartAddr & 4095) != 0)
    {
        return ERROR_UNALIGNED_ADDRESS;
    }

    a_paging->pagingStruct = (void*) &g_kernelPagingStruct;
    g_kernelPagingStruct.currentPageDirectory = &g_kernelPageDirectory;

    kmemset(g_kernelPageDirectory.entries, 0, sizeof(struct IA32_PageDirectory_4KB_Entry) * PAGING_IA32_PDE_NUMBER);
    for (size_t i = 0; i < PAGING_IA32_PDE_NUMBER; i++)
    {
        g_kernelPageDirectory.addresses[i] = NULL;
    }

    size_t pages = (a_kernelEndAddr - a_kernelStartAddr) / (4 * KiB);
    kprintf("Start: %x End: %x Pages: %d\n", a_kernelStartAddr, a_kernelEndAddr, pages);
    IA32_4KB_allocAtAddress(a_paging, a_kernelStartAddr, pages, 0, true, true, false, false);

    return ERROR_SUCCESS;
}

size_t IA32_4KB_alloc(struct Paging *a_paging,
                      size_t         a_pagesNumber,
                      uint32         a_flags,
                      bool           a_write,
                      bool           a_user,
                      bool           a_writeThrough,
                      bool           a_cacheDisabled,
                      size_t        *a_address)
{
    if (a_paging == NULL || a_address == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    struct IA32_Paging_4KB *currentPagingStruct = (struct IA32_Paging_4KB*) a_paging->pagingStruct;
    struct IA32_PageDirectory_4KB *currentPageDirectory = currentPagingStruct->currentPageDirectory;

    size_t address;
    PMM_alloc(&address, a_pagesNumber, PMM_FOR_VIRTUAL_MEMORY);

    for (size_t i = 0; i < a_pagesNumber; i++, address += 4 * KiB)
    {
        helper_IA32_4KB_allocPage(currentPageDirectory, address, a_flags,
                                a_write, a_user, a_writeThrough, a_cacheDisabled);
    }

    return ERROR_SUCCESS;
}

size_t IA32_4KB_allocAtAddress(struct Paging *a_paging,
                               size_t         a_address,
                               size_t         a_pagesNumber,
                               uint32         a_flags,
                               bool           a_write,
                               bool           a_user,
                               bool           a_writeThrough,
                               bool           a_cacheDisabled)
{
    if (a_paging == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    struct IA32_Paging_4KB *currentPagingStruct = (struct IA32_Paging_4KB*) a_paging->pagingStruct;
    struct IA32_PageDirectory_4KB *currentPageDirectory = currentPagingStruct->currentPageDirectory;

    for (size_t i = 0, address = a_address; i < a_pagesNumber; i++, address += 4 * KiB)
    {
        helper_IA32_4KB_allocPage(currentPageDirectory, address, a_flags,
                                a_write, a_user, a_writeThrough, a_cacheDisabled);
    }

    return ERROR_SUCCESS;
}

size_t IA32_4KB_free(struct Paging *a_paging,
                     size_t         a_address,
                     size_t         a_pagesNumber,
                     uint32         a_flags)
{
    if (a_paging == NULL || a_address == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    PMM_free(a_address, a_pagesNumber, PMM_FOR_VIRTUAL_MEMORY);
    for (size_t i = 0, address = a_address; i < a_pagesNumber; i++, address += 4 * KiB)
    {
        helper_IA32_4KB_freePage(a_paging, address);
    }

    return ERROR_SUCCESS;
}
*/
