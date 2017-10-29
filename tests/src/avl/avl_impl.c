#include "tests/src/avl/avl_impl.h"

IMPLEMENT_AVL_TYPE(Data, UTData);

static size_t g_memInUsage = 0;
static size_t g_objectsInUsage = 0;
static size_t g_allocCalls = 0;
static size_t g_freeCalls = 0;

void* AVLAllocNode(size_t a_size)
{
    g_memInUsage += a_size;

    g_allocCalls++;

    return malloc(a_size);
}

void AVLFreeNode(void *a_node)
{
    g_memInUsage -= sizeof(UTDataAVLNode);

    g_freeCalls++;

    return free(a_node);
}

void AVLDestroyData(Data *a_data)
{
    g_objectsInUsage--;

    a_data->data = 0;
}

avl_error_code_t AVLCopyData(Data *dest, const Data *src)
{
    g_objectsInUsage++;

    dest->data = src->data;

    return AVL_ERROR_SUCCESS;
}

size_t GetMemoryInUsage()
{
    return g_memInUsage;
}

void ResetMemoryInUsage()
{
    g_memInUsage = 0;
}

size_t GetObjectsInUsage()
{
    return g_objectsInUsage;
}

void ResetObjectsInUsage()
{
    g_objectsInUsage = 0;
}

size_t GetAllocCalls()
{
    return g_allocCalls;
}

void ResetAllocCalls()
{
    g_allocCalls = 0;
}

size_t GetFreeCalls()
{
    return g_freeCalls;
}

void ResetFreeCalls()
{
    g_freeCalls = 0;
}