#include "drivers/acpi/include/acpi_srat.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "util/kstdlib/include/kstring.h"
#endif

static bool acpi_srat_doChecksum(
    uint8* a_ptr,
    uint32 a_length)
{
    uint8 sum = 0;
    for (uint32 i = 0; i < a_length; i++) {
        sum += a_ptr[i];
    }

    if (sum == 0) {
        return true;
    }

    return false;
}

uint32 acpi_srat_init(
    PSRAT a_srat,
    uint8 *a_ptr)
{
    if (a_srat == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_ptr == NULL) {
        return ERROR_NULL_POINTER;
    }

    uint32 error = kmemcpy(&a_srat->header, a_ptr, sizeof(SRATHeader));
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    error = SDTHeader_checkSignature(&a_srat->header.sdt, "SRAT");
    if (error != ERROR_SUCCESS) {
        return ERROR_NOT_FOUND;
    }

    if (acpi_srat_doChecksum(a_ptr, a_srat->header.sdt.length) == false) {
        return ERROR_NOT_FOUND;
    }

    a_srat->entries = a_ptr + sizeof(SRATHeader);

    return ERROR_SUCCESS;
}

uint32 acpi_srat_getNumberOfSRA(
    PSRAT a_srat,
    uint32 a_type,
    uint32 *a_number)
{
    if (a_srat == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_srat->entries == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_number == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (a_type != SRAT_PROCESSOR_LOCAL_APIC_TYPE &&
        a_type != SRAT_MEMORY_TYPE &&
        a_type != SRAT_PROCESSOR_LOCAL_x2APIC_TYPE) {
        return ERROR_UNKNOWN;
    }

    *a_number = 0;
    uint8 *ptr = a_srat->entries;
    uint32 length = a_srat->header.sdt.length - sizeof(SRATHeader);

    for (uint32 i = 0; i < length;) {
        if (ptr[0] == a_type) {
            (*a_number)++;
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

    uint32 length = a_srat->header.sdt.length - sizeof(SRATHeader);
    uint8 *ptr = a_srat->entries;
    for (uint32 i = 0; i < length;) {
        if (ptr[0] == a_type) {
            a_position--;
            if (a_position == 0) {
                if (kmemcpy(a_buffer, ptr, a_bufferSize) == ERROR_SUCCESS) {
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
