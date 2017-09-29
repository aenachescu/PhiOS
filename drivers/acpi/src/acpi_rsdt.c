#include "drivers/acpi/include/acpi_rsdt.h"
#include "util/kstdlib/include/kstring.h"

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

    uint32 error = kmemcpy(rsdt, ptr, sizeof(RSDT));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&rsdt->header, "RSDT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    rsdt->entries = (uint32*) (ptr + sizeof(RSDT));

    return ERROR_SUCCESS;
}

uint32 acpi_rsdt_create(
    PRSDT rsdt,
    uint8* ptr)
{

}
