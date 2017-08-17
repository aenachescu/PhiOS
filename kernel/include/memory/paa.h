#ifndef PhiOS_PlacementAddressAllocator
#define PhiOS_PlacementAddressAllocator

#include "include/errors.h"
#include "include/types.h"

/*
 * @brief Initializes the placement address allocator.
 *
 * @param a_startAddress The address where start the allocating.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully
 *  ERROR_ALREADY_INITIALIZED - if the placement address allocator is already
 *      initialized
 */
size_t PAA_init(
    size_t a_startAddress
);

/*
 * @brief This function allocates memory at the end of the kernel. It should be
 *        used just until is initialized the paging.
 *
 * @param a_size The memory size that must be allocated.
 * @param a_address If the function ends successfully then in a_address will be
 *                  stored the starting address of the allocated memory area.
 * @param a_alignment The alignment size.
 *
 * @rerturn
 *  ERROR_SUCCESS - If the function ends successfully.
 *  ERROR_UNINITIALIZED - If PAA is not initialized.
 *  ERROR_INVALID_PARAMETER - If a_size is 0.
 *  ERROR_NULL_POINTER - If a_address is null.
 */
size_t PAA_alloc(
    size_t a_size, 
    size_t *a_address, 
    size_t a_alignment
);

/*
 * @brief Get current address for placement
 *
 * @return
 *  Returns the current placement address or 0 if the PAA is not initialized
 */
size_t PAA_getCurrentAddress();

#endif
