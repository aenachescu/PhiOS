#include "drivers/acpi/include/acpi.h"

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
    return ERROR_NOT_FOUND;
}