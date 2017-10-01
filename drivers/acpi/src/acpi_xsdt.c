#include "drivers/acpi/include/acpi_xsdt.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "util/kstdlib/include/kstring.h"
#include "kernel/include/memory/paa.h"
#include "kernel/include/logging.h"
#endif

uint32 acpi_xsdt_init(
    PXSDT xsdt,
    uint8* ptr)
{
    if (xsdt == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (ptr == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 error = kmemcpy(&xsdt->header, ptr, sizeof(SDTHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&xsdt->header, "XSDT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    xsdt->entries = (uint64*) (ptr + sizeof(SDTHeader));

    return ERROR_SUCCESS;
}

uint32 acpi_xsdt_create(
    PXSDT xsdt,
    uint8* ptr)
{
    if (xsdt == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (ptr == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 error = kmemcpy(&xsdt->header, ptr, sizeof(SDTHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&xsdt->header, "XSDT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    uint64 address;
    uint32 entriesNum = (xsdt->header.length - sizeof(SDTHeader)) / sizeof(uint64);
    uint32 entriesSize = entriesNum * sizeof(uint64);

    error = PAA_alloc(entriesSize, &address, sizeof(uint64));
    if (error != ERROR_SUCCESS) {
        return ERROR_ALLOC_ERROR;
    }

#ifdef PhiOS32
    if (address + entriesSize > 0xFFFFFFFFULL) {
        return ERROR_ALLOC_ERROR;
    }

    xsdt->entries = (uint64*) ((uint32) address);
#elif defined PhiOS64

    xsdt->entries = (uint64*) address;

#else

#error "Unsupported architecture"

#endif

    error = kmemcpy(xsdt->entries, ptr + sizeof(SDTHeader), entriesSize);
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    return ERROR_SUCCESS;
}

uint32 acpi_xsdt_check32BitsCompatibility(
    PXSDT xsdt)
{
    if (xsdt == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 entriesNum = (xsdt->header.length - sizeof(SDTHeader)) / sizeof(uint64);
    for (uint32 i = 0; i < entriesNum; i++) {
        // we need 4 bytes to check signature, that's why
        // we use 0xFFFFFFFC instead of 0xFFFFFFFF
        if (xsdt->entries[i] > 0xFFFFFFFCULL) {
            KLOG_WARNING("Addr: %llu", xsdt->entries[i]);
            return ERROR_INCOMPATIBLE;
        }
    }

    return ERROR_SUCCESS;
}

uint32 acpi_xsdt_findHeader(
    PXSDT xsdt,
    const char* headerSignature,
    uint64* address)
{
    if (xsdt == NULL) {
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
    uint32 entriesNum = (xsdt->header.length - sizeof(SDTHeader)) / 8;
    for (uint32 i = 0; i < entriesNum; i++) {
        void *addr = (void*) ((size_t) xsdt->entries[i]);
        if (kmemcmp(addr, headerSignature, 4, &cmpRes) == ERROR_SUCCESS) {
            if (cmpRes == 0) {
                *address = xsdt->entries[i];
                return ERROR_SUCCESS;
            }
        }
    }

    return ERROR_NOT_FOUND;
}
