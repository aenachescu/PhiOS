#ifndef PhiOS_DRIVERS_ACPI_SDT_HEADER
#define PhiOS_DRIVERS_ACPI_SDT_HEADER

#include "include/types.h"
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

#endif
