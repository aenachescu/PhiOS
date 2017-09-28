#ifndef PhiOS_DRIVERS_ACPI
#define PhiOS_DRIVERS_ACPI

#include "include/types.h"
#include "include/errors.h"

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
} __attribute__ ((packed));

typedef struct _RSDP   RSDP;
typedef struct _RSDP* PRSDP;


struct _RSDP2
{
    RSDP   rsdp;

    uint32 length;
    uint64 xsdtAddr;
    uint8  checksum;
    uint8  reserved[3];
} __attribute__ ((packed));

typedef struct _RSDP2   RSDP2;
typedef struct _RSDP2* PRSDP2;

uint32 acpi_getRSDP(
    PRSDP2 rsdp2,
    uint32 *acpiVersion
);

#endif
