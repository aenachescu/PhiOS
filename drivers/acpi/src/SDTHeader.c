#include "drivers/acpi/include/SDTHeader.h"
#include "drivers/config.h"

#ifdef STATIC_LINKAGE
#include "util/kstdlib/include/kstring.h"
#endif

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

    return ERROR_SUCCESS;
}