#include "drivers/acpi/include/acpi_rsdt.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "util/kstdlib/include/kstring.h"
#include "kernel/include/memory/paa.h"
#endif

static bool acpi_rsdt_doChecksum(
    uint8* ptr,
    uint32 length)
{
    uint8 sum = 0;
    for (uint32 i = 0; i < length; i++) {
        sum += ptr[i];
    }

    if (sum == 0) {
        return true;
    }

    return false;
}

uint32 acpi_rsdt_init(
    PRSDT rsdt,
    uint8* ptr)
{
    if (rsdt == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (ptr == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 error = kmemcpy(&rsdt->header, ptr, sizeof(SDTHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&rsdt->header, "RSDT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    if (acpi_rsdt_doChecksum(ptr, rsdt->header.length) == false) {
        return ERROR_NOT_FOUND;
    }

    rsdt->entries = (uint32*) (ptr + sizeof(SDTHeader));

    return ERROR_SUCCESS;
}

uint32 acpi_rsdt_create(
    PRSDT rsdt,
    uint8* ptr)
{
    if (rsdt == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (ptr == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 error = kmemcpy(&rsdt->header, ptr, sizeof(SDTHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&rsdt->header, "RSDT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    if (acpi_rsdt_doChecksum(ptr, rsdt->header.length) == false) {
        return ERROR_NOT_FOUND;
    }

    uint64 address;
    uint32 entriesNum = (rsdt->header.length - sizeof(SDTHeader)) / sizeof(uint32);
    uint32 entriesSize = entriesNum * sizeof(uint32);

    error = PAA_alloc(entriesSize, &address, sizeof(uint32));
    if (error != ERROR_SUCCESS) {
        return ERROR_ALLOC_ERROR;
    }

    if (address + entriesSize > 0xFFFFFFFFULL) {
        return ERROR_ALLOC_ERROR;
    }

    rsdt->entries = (uint32*) ((uint32) address);

    error = kmemcpy(rsdt->entries, ptr + sizeof(SDTHeader), entriesSize);
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    return ERROR_SUCCESS;
}

uint32 acpi_rsdt_findHeader(
    PRSDT rsdt,
    const char* headerSignature,
    uint32* address)
{
    if (rsdt == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (headerSignature == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (address == NULL) {
        return ERROR_NULL_POINTER;
    }

    *address = NULL;

    sint32 cmpRes = 0;
    uint32 entriesNum = (rsdt->header.length - sizeof(SDTHeader)) / 4;
    for (uint32 i = 0; i < entriesNum; i++) {
        void *addr = (void*) rsdt->entries[i];
        if (kmemcmp(addr, headerSignature, 4, &cmpRes) == ERROR_SUCCESS) {
            if (cmpRes == 0) {
                *address = rsdt->entries[i];
                return ERROR_SUCCESS;
            }
        }
    }

    return ERROR_NOT_FOUND;
}
