#ifndef PhiOS_PlacementAddressAllocator
#define PhiOS_PlacementAddressAllocator

#include "errors.h"
#include "types.h"

/*
 * @brief Initializes the placement address allocator.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully
 *  ERROR_ALREADY_INITIALIZED - if the placement address allocator is already
 *      initialized
 */
size_t PAA_init(void);

/*
 * @brief This function allocates memory at the end of the kernel. It should be
 *        used just until is initialized the paging.
 *
 * @param a_size The memory size that must be allocated.
 * @param a_address If the function ends successfully then in a_address will be
 *                  stored the starting address of the allocated memory area.
 *
 * @rerturn
 *  ERROR_SUCCESS - If the function ends successfully.
 *  ERROR_UNINITIALIZED - If PAA is not initialized.
 *  ERROR_INVALID_PARAMETER - If a_size is 0.
 *  ERROR_NULL_POINTER - If a_address is null.
 */
size_t PAA_alloc(size_t a_size, size_t *a_address);

#endif
