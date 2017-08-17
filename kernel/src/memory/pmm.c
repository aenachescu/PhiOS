#include "pmm.h"
#include "paa.h"

struct PMA *g_allocators        = NULL;
uint8       g_index             = 0;
uint8       g_allocatorsNumber  = 0;

uint32 PMM_init(
    uint8 a_allocatorsNumber)
{
    size_t address = 0;
    uint32 error = ERROR_SUCCESS;

    do {
        if (g_allocators != NULL) {
            error = ERROR_ALREADY_INITIALIZED;
            break;
        }

        if (a_allocatorsNumber == 0) {
            error = ERROR_INVALID_PARAMETER;
            break;
        }

        error = PAA_alloc(
            sizeof(struct PMA) * a_allocatorsNumber,
            &address,
            WORDSIZE_BYTES
        );
        if (error != ERROR_SUCCESS) {
            break;
        }

        g_allocators        = (struct PMA*) address;
        g_index             = 0;
        g_allocatorsNumber  = a_allocatorsNumber;
    } while (false);

    return error;
}

uint32 PMM_addAllocator(
    void *a_pma,
    uint8 a_flag,
    PMA_ALLOC_PFN a_allocFn,
    PMA_FREE_PFN a_freeFn,
    PMA_RESERVE_PFN a_reserveFn)
{
    uint32 error = ERROR_SUCCESS;

    do {
        if (g_allocators == NULL) {
            error = ERROR_UNINITIALIZED;
            break;
        }

        if (g_index == g_allocatorsNumber) {
            error = ERROR_LIMIT_REACHED;
            break;
        }

        if (a_pma == NULL) {
            error = ERROR_NULL_POINTER;
            break;
        }

        if (a_allocFn == NULL || a_freeFn == NULL || a_reserveFn == NULL) {
            error = ERROR_INVALID_FUNCTION;
            break;
        }

        g_allocators[g_index].type        = a_flag;
        g_allocators[g_index].locked      = 0;
        g_allocators[g_index].PMAStruct   = a_pma;
        g_allocators[g_index].allocFn     = a_allocFn;
        g_allocators[g_index].freeFn      = a_freeFn;
        g_allocators[g_index].reserveFn   = a_reserveFn;
        g_index++;
    } while (false);

    return error;
}

uint32 PMM_alloc(
    uint64 *a_address,
    uint64 a_size,
    uint8 a_flag)
{
    uint32 error = ERROR_UNKNOWN_FLAG;

    do {
        if (g_allocators == NULL) {
            error = ERROR_UNINITIALIZED;
            break;
        }

        for (uint8 i = 0; i < g_index; i++) {
            if (g_allocators[i].type != a_flag) {
                continue;
            }

            /*
             * TODO when will be implemented the multitasking.
             * Here should checked if the structure is locked or not.
             */

            error = g_allocators[i].allocFn(
                (void*) g_allocators[i].PMAStruct,
                a_size,
                a_address
            );
            if (error == ERROR_SUCCESS) {
                break;
            }
        }
    } while (false);

    return error;
}

uint32 PMM_free(
    uint64 a_address,
    uint64 a_size,
    uint8 a_flag)
{
    uint32 error = ERROR_UNKNOWN_FLAG;

    do {
        if (g_allocators == NULL) {
            error = ERROR_UNINITIALIZED;
            break;
        }

        for (uint8 i = 0; i < g_index; i++) {
            if (g_allocators[i].type != a_flag) {
                continue;
            }

            /*
             * TODO when will be implemented the multitasking.
             * Here should checked if the structure is locked or not.
             */

            error = g_allocators[i].freeFn(
                (void*) g_allocators[i].PMAStruct,
                a_size,
                a_address
            );
            if (error == ERROR_SUCCESS) {
                break;
            }
        }
    } while (false);

    return error;
}

uint32 PMM_reserve(
    uint64 a_address,
    uint64 a_size,
    uint8 a_flag)
{
    uint32 error = ERROR_UNKNOWN_FLAG;

    do {
        if (g_allocators == NULL) {
            error = ERROR_UNINITIALIZED;
            break;
        }

        for (uint8 i = 0; i < g_index; i++) {
            if (g_allocators[i].type != a_flag) {
                continue;
            }

            /*
             * TODO when will be implemented the multitasking.
             * Here should checked if the structure is locked or not.
             */

            error = g_allocators[i].reserveFn(
                (void*) g_allocators[i].PMAStruct,
                a_size,
                a_address
            );
            if (error == ERROR_SUCCESS){
                break;
            }
        }
    } while (false);

    return error;
}
