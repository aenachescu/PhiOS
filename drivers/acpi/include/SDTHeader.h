#ifndef PhiOS_DRIVERS_ACPI_SDT_HEADER
#define PhiOS_DRIVERS_ACPI_SDT_HEADER

#include "include/types.h"
#include "include/errors.h"
#include "include/compiler.h"

struct _SDTHeader
{
    char    signature[4];
    uint32  length;
    uint8   revision;
    uint8   checksum;
    char    OEMID[6];
    char    OEMTableID[8];
    uint32  OEMRevision;
    uint32  creatorID;
    uint32  creatorRevision;
} PhiOS_PACKED_STRUCTURE;

typedef struct _SDTHeader   SDTHeader;
typedef struct _SDTHeader* PSDTHeader;

/*
 * @brief: Checks if it's a header for a particular structure.
 *
 * @param:
 *  header - pointer to the header which to be verified.
 *  signature - the expected signature.
 *
 * @return:
 *  ERROR_SUCCESS - it's the expected header.
 *  ERROR_NULL_POINTER - signature or header are null.
 *  ERROR_NOT_FOUND - the header has not the expected signature.
 *  ERROR_INTERNAL_ERROR - the signatures could not be compared.
 */
uint32 SDTHeader_checkSignature(
    const PSDTHeader header,
    const char* signature
);

#endif
