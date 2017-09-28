#ifndef PhiOS_DRIVERS_ACPI_XSDT
#define PhiOS_DRIVERS_ACPI_XSDT

#include "include/types.h"
#include "include/errors.h"
#include "include/compiler.h"

#include "drivers/acpi/include/SDTHeader.h"

struct _XSDT
{
    SDTHeader   header;     // the header of the XSDT structure.
    uint64     *entries;    // pointers to other SDT headers.
} PhiOS_PACKED_STRUCTURE;

typedef struct _XSDT   XSDT;
typedef struct _XSDT* PXSDT;

/*
 * @brief: Initializes XSDT structure. This function does not alloc memory for
 *      entries field, it just sets a pointer to memory area from ptr where
 *      the entries are stored.
 *
 * @param:
 *  xsdt - the structure which is initialized.
 *  ptr - the memory area from where the structure is initialized.
 *
 * @return:
 */
uint32 acpi_xsdt_init(
    PXSDT xsdt,
    uint8 *ptr
);

/*
 * @brief: Creates XSDT structure. This function alloc memory for entries field
 *      and copy them from ptr into entries field.
 *
 * @param:
 *  xsdt - the structure which is created.
 *  ptr - the memory area from where the structure is created.
 *
 * @return:
 */
uint32 acpi_xsdt_create(
    PXSDT xsdt,
    uint8* ptr
);

/*
 * @brief: Checks if the pointers from entries are 32-bit compatible.
 *
 * @param:
 *  xsdt - the structure which is verified.
 *
 * @return:
 *  ERROR_SUCCESS - the pointers are 32-bit compatible.
 *  ERROR_INCOMPATIBLE - the pointer are NOT 32-bit compatible.
 *  ERROR_NULL_POINTER - xsdt pointer is null.
 */
uint32 acpi_xsdt_check32BitsCompatibility(
    PXSDT xsdt
);

#endif
