#include "types.h"
#include "arch/x86/gdt32.h"
#include "arch/x86/tss32.h"
#include "errors.h"

static struct GDT32         g_GDT32Struct[GDT_ENTRIES];
static struct GDT32_Entry   g_GDT32Entries[GDT_ENTRIES];
static struct GDT32_Pointer g_GDT32Pointer;
extern struct TSS32_Entry   g_TSSKernelEntry;

size_t GDT32_init()
{
    GDT32_setStruct(&g_GDT32Struct[0], 0, 0, 0);
    GDT32_setStruct(&g_GDT32Struct[1], 0, 0xFFFFFFFF, GDT_CODE_PL0);
    GDT32_setStruct(&g_GDT32Struct[2], 0, 0xFFFFFFFF, GDT_DATA_PL0);
    GDT32_setStruct(&g_GDT32Struct[3], 0, 0xFFFFFFFF, GDT_CODE_PL3);
    GDT32_setStruct(&g_GDT32Struct[4], 0, 0xFFFFFFFF, GDT_DATA_PL3);

    TSS32_init(0x10, 0x0);
    uint32 tssBase = (uint32) &g_TSSKernelEntry;
    uint32 tsslimit = tssBase + sizeof(struct TSS32_Entry);
    GDT32_setStruct(&g_GDT32Struct[5], tssBase, tsslimit, TSS_FLAGS);

    GDT32_createEntries(g_GDT32Struct);

    g_GDT32Pointer.limit = sizeof(struct GDT32_Entry) * GDT_ENTRIES - 1;
    g_GDT32Pointer.base = (uint32) &g_GDT32Entries;

    GDT32_Load((uint32) &g_GDT32Pointer);
    TSS32_Load();

    return ERROR_SUCCESS;
}

size_t GDT32_setStruct(struct GDT32 *a_gdt, uint32 a_base,
                       uint32 a_limit, uint16 a_type)
{
    if (a_gdt == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    a_gdt->base = a_base;
    a_gdt->limit = a_limit;
    a_gdt->type = a_type;

    return ERROR_SUCCESS;
}

size_t GDT32_getStruct(uint32 a_num, struct GDT32 **a_gdt)
{
    if (a_num >= GDT_ENTRIES)
    {
        return ERROR_UNSUPPORTED;
    }

    if (a_gdt == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    *a_gdt = &g_GDT32Struct[a_num];
    return ERROR_SUCCESS;
}

size_t GDT32_createEntries(struct GDT32 *a_gdtArray)
{
    if (a_gdtArray == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    for (int i = 0; i < GDT_ENTRIES; i++)
    {
        struct GDT32_Entry descriptor;
        uint32 base = a_gdtArray[i].base;
        uint32 limit = a_gdtArray[i].limit;
        uint16 type = a_gdtArray[i].type;

        descriptor.data  = limit        & 0x000F0000;
        descriptor.data |= (type << 8)  & 0x00F0FF00;
        descriptor.data |= (base >> 16) & 0x000000FF;
        descriptor.data |= base         & 0xFF000000;

        descriptor.data <<= 32;

        descriptor.data |= base  << 16;
        descriptor.data |= limit  & 0x0000FFFF;

        g_GDT32Entries[i] = descriptor;
    }

    return ERROR_SUCCESS;
}
