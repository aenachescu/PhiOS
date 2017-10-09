#include "drivers/acpi/include/acpi_srat.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "util/kstdlib/include/kstring.h"
#endif

static bool acpi_srat_doChecksum(
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

uint32 acpi_srat_init(
    PSRAT srat,
    uint8 *ptr)
{
    if (srat == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (ptr == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 error = kmemcpy(&srat->header, ptr, sizeof(SRATHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&srat->header.sdt, "SRAT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    if (acpi_srat_doChecksum(ptr, srat->header.sdt.length) == false) {
        return ERROR_NOT_FOUND;
    }

    srat->entries = ptr + sizeof(SRATHeader);

    return ERROR_SUCCESS;
}
