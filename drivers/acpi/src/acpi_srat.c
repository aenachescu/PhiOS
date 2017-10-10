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

uint32 acpi_srat_getNumberOfSRA(
    PSRAT srat,
    uint32 type,
    uint32 *number)
{
    if (srat == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (srat->entries == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (number == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (type != SRAT_PROCESSOR_LOCAL_APIC_TYPE &&
        type != SRAT_MEMORY_TYPE &&
        type != SRAT_PROCESSOR_LOCAL_x2APIC_TYPE)
    {
        return ERROR_UNKNOWN;
    }

    *number = 0;
    uint8 *ptr = srat->entries;
    uint32 length = srat->header.sdt.length - sizeof(SRATHeader);

    for (uint32 i = 0; i < length; i++) {
        if (ptr[0] == type) {
            (*number)++;
        }

        i += ptr[1];
        ptr += ptr[1];
    }

    return ERROR_SUCCESS;
}

uint32 acpi_srat_getNthSRAStructure(
    PSRAT a_srat,
    void *a_buffer,
    uint32 a_bufferSize,
    uint32 a_type,
    uint32 a_position)
{
    if (a_srat == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_buffer == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 length = srat->header.sdt.length - sizeof(SRATHeader);
    uint8 *ptr = srat->entries;
    for (uint32 i = 0; i < length;) {
        if (ptr[0] == a_type) {
            a_position--;
            if (a_position == 0) {
                if (kmemcpy(a_buffer, ptr, a_size) == ERROR_SUCCESS) {
                    return ERROR_SUCCESS;
                }

                return ERROR_INTERNAL_ERROR;
            }
        }

        i += ptr[1];
        ptr += ptr[1];
    }

    return ERROR_NOT_FOUND;
}
