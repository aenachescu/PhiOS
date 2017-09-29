#include "drivers/acpi/include/SDTHeader.h"
#include "util/kstdlib/include/kstring.h"

uint32 SDTHeader_checkSignature(
    const PSDTHeader header,
    const char* signature)
{
    if (header == NULL) {
        return ERROR_NULL_POINTER;
    }

    if (signature == NULL) {
        return ERROR_NULL_POINTER;
    }

    sint32 memcmpRes;
    uint32 error = kmemcmp(header->signature, signature, 4, &memcmpRes);
    if (error != ERROR_SUCCESS) {
        return ERROR_INTERNAL_ERROR;
    }

    if (memcmpRes != 0) {
        return ERROR_NOT_FOUND;
    }

    uint8 *ptr = (uint8*) header;
    uint8 sum = 0;
    for (size_t i = 0; i < sizeof(SDTHeader); i++) {
        sum += ptr[i];
    }

    if (sum != 0) {
        return ERROR_NOT_FOUND;
    }

    return ERROR_SUCCESS;
}