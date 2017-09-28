#ifndef PhiOS_DRIVERS_ACPI_RSDT
#define PhiOS_DRIVERS_ACPI_RSDT

#include "include/types.h"
#include "include/errors.h"
#include "include/compiler.h"

#include "drivers/acpi/include/SDTHeader.h"

struct _RSDT
{
    SDTHeader   header;     // the header of the RSDT structure.
    uint32     *entries;    // pointers to other SDT headers.
} PhiOS_PACKED_STRUCTURE;

typedef struct _RSDT   RSDT;
typedef struct _RSDT* PRSDT;

/*
 * @brief: Initializes RSDT structure. This function does not alloc memory for
 *      entries field, it just sets a pointer to memory area from ptr where
 *      the entries are stored.
 *
 * @param:
 *  rsdt - the structure which is initialized.
 *  ptr - the memory area from where the structure is initialized.
 *
 * @return:
 */
uint32 acpi_rsdt_init(
    PRSDT rsdt,
    uint8 *ptr
);

/*
 * @brief: Creates RSDT structure. This function alloc memory for entries field
 *      and copy them from ptr into entries field.
 *
 * @param:
 *  rsdt - the structure which is created.
 *  ptr - the memory area from where the structure is created.
 *
 * @return:
 */
uint32 acpi_rsdt_create(
    PRSDT rsdt,
    uint8* ptr
);

#endif
