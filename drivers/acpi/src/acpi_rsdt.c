#include "drivers/acpi/include/acpi_rsdt.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "util/kstdlib/include/kstring.h"
#include "kernel/include/memory/paa.h"
#endif

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

    error = kmemcpy(rsdt->entries, ptr + sizeof(SDTHeader), sizeof(SDTHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    return ERROR_SUCCESS;
}
