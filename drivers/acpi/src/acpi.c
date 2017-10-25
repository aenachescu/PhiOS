#include "drivers/acpi/include/acpi.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "kernel/include/logging.h"
#include "util/kstdlib/include/kstring.h"
#endif

uint32 acpi_getRSDP(
    PRSDP2 rsdp2,
    uint32 *acpiVersion)
{
    if (rsdp2 == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (acpiVersion == NULL) {
        return ERROR_NULL_POINTER;
    }

    *acpiVersion = ACPI_VERSION_UNKNOWN;

    /*
     * TODO:
     *  Checks if it's uefi or legacy system.
     *      1. It's uefi system - get address from EFI_SYSTEM_TABLE.
     *      2. It's legacy system:
     *          2a. Search RSDP/RSDP2 in the memory area from 0x000E0000 to 0x000FFFFF
     *          2b. Search RSDP/RSDP2 in the first 1KiB of the EBDA.
     */

    uint8 *ptr = (uint8*) 0x000E0000;
    sint32 cmpRes = 0;
    uint8 checksum = 0;

    for (; ptr < (uint8*)0x00100000; ptr += 16) {
        if (kmemcmp(ptr, "RSD PTR ", 8, &cmpRes) != ERROR_SUCCESS) {
            continue;
        }

        if (cmpRes != 0) {
            continue;
        }

        // checks checksum for RSDP2
        checksum = 0;
        for (size_t i = 0; i < sizeof(RSDP2); i++) {
            checksum += ptr[i];
        }

        if (checksum == 0) {
            *acpiVersion = ACPI_VERSION_2;
            break;
        }

        // checks checksum for RSDP
        checksum = 0;
        for (size_t i = 0; i < sizeof(RSDP); i++) {
            checksum += ptr[i];
        }

        if (checksum == 0) {
            *acpiVersion = ACPI_VERSION_1;
            break;
        }
    }

    if (*acpiVersion == ACPI_VERSION_UNKNOWN) {
        KLOG_INFO("Not found RSDP");
        return ERROR_NOT_FOUND;
    }

    KLOG_INFO("Found RSDP at [%0P], version = %u", ptr, *acpiVersion);

    switch (*acpiVersion) {
        case ACPI_VERSION_1:
            if (kmemcpy(rsdp2, ptr, sizeof(RSDP)) != ERROR_SUCCESS) {
                return ERROR_INTERNAL_ERROR;
            }
            break;

        case ACPI_VERSION_2:
            if (kmemcpy(rsdp2, ptr, sizeof(RSDP2)) != ERROR_SUCCESS) {
                return ERROR_INTERNAL_ERROR;
            }
            break;

        default:
            *acpiVersion = ACPI_VERSION_UNKNOWN;
            return ERROR_UNKNOWN_VERSION;
    }

    return ERROR_SUCCESS;
}
