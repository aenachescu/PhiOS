#include "types.h"
#include "cpu.h"
#include "errors.h"
#include "arch/x86/paging32.h"

PageDirectory32 *g_kernelPageDirectory;

size_t paging32_init()
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_switchPageDirectory(PageDirectory32 *a_dir)
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_map(uint32 a_virtual, uint32 a_physical, uint32 a_flags)
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_unmap(uint32 a_virtual)
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_mapArea(uint32 a_startVirtual, uint32 a_endVitual,
                        uint32 a_startPhysical, uint32 a_flags)
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_unmapArea(uint32 a_startVirtual, uint32 a_endVitual)
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_allocPage(uint32 a_virtual, uint32 a_flags)
{
    return ERROR_UNSUPPORTED;
}

size_t paging32_freePage(uint32 a_virtual)
{
    return ERROR_UNSUPPORTED;
}
