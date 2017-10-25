#ifndef PhiOS_DRIVERS_ACPI
#define PhiOS_DRIVERS_ACPI

#include "include/types.h"
#include "include/errors.h"

#include "include/compiler.h"

#define ACPI_VERSION_UNKNOWN    0
#define ACPI_VERSION_1          1
#define ACPI_VERSION_2          2

struct _RSDP
{
    char   signature[8];
    uint8  checksum;
    char   OEMID[6];
    uint8  revision;
    uint32 rsdtAddr;
} PhiOS_PACKED_STRUCTURE;

typedef struct _RSDP   RSDP;
typedef struct _RSDP* PRSDP;


struct _RSDP2
{
    RSDP   rsdp;

    uint32 length;
    uint64 xsdtAddr;
    uint8  checksum;
    uint8  reserved[3];
} PhiOS_PACKED_STRUCTURE;

typedef struct _RSDP2   RSDP2;
typedef struct _RSDP2* PRSDP2;

/*
 * @brief: Get root system description pointer.
 *
 * @param:
 *  rsdp2 - pointer to RSDP struct where the structure is moved if it's found.
 *      The data in the structure is not modified if RSDP was not found.
 *  acpiVersion - here it's stored the version of the RSDP.
 *      ACPI_VERSION_UNKNOWN it's stored if RSDP is not found.
 *
 * @return:
 *  ERROR_SUCCESS - RSDP was found.
 *  ERROR_NULL_POINTER - rsdp2 or acpiVersion are null.
 *  ERROR_NOT_FOUND - RSDP was not found.
 *  ERROR_UNKNOWN_VERSION - unknown version of RSDP.
 *  ERROR_INTERNAL_ERROR - something went wrong.
 */
uint32 acpi_getRSDP(
    PRSDP2 rsdp2,
    uint32 *acpiVersion
);

#endif
