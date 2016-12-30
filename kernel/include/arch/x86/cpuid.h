#ifndef PhiOS_CPUID
#define PhiOS_CPUID

#include "types.h"
#include "errors.h"

/*
 * These defines are for CPUID EAX = 01H, features from ECX
 */
#define CPU_FEATURE_SSE3                0x00000100
#define CPU_FEATURE_PCLMULQDQ           0x00000101
#define CPU_FEATURE_DTES64              0x00000102
#define CPU_FEATURE_MONITOR             0x00000103
#define CPU_FEATURE_DS_CPL              0x00000104
#define CPU_FEATURE_WMX                 0x00000105
#define CPU_FEATURE_SMX                 0x00000106
#define CPU_FEATURE_EIST                0x00000107
#define CPU_FEATURE_TM2                 0x00000108
#define CPU_FEATURE_SSSE3               0x00000109
#define CPU_FEATURE_CNXT_ID             0x0000010A
#define CPU_FEATURE_SDBG                0x0000010B
#define CPU_FEATURE_FMA                 0x0000010C
#define CPU_FEATURE_CMPXCHG16B          0x0000010D
#define CPU_FEATURE_xTPR                0x0000010E
#define CPU_FEATURE_PDCM                0x0000010F
//#define CPU_FEATURE_RESERVED            0x00000110
#define CPU_FEATURE_PCID                0x00000111
#define CPU_FEATURE_DCA                 0x00000112
#define CPU_FEATURE_SSE4_1              0x00000113
#define CPU_FEATURE_SSE4_2              0x00000114
#define CPU_FEATURE_x2APIC              0x00000115
#define CPU_FEATURE_MOVBE               0x00000116
#define CPU_FEATURE_POPCNT              0x00000117
#define CPU_FEATURE_TSC_DEADLINE        0x00000118
#define CPU_FEATURE_AESNI               0x00000119
#define CPU_FEATURE_XSAVE               0x0000011A
#define CPU_FEATURE_OSXSAVE             0x0000011B
#define CPU_FEATURE_AVX                 0x0000011C
#define CPU_FEATURE_F16C                0x0000011D
#define CPU_FEATURE_RDRAND              0x0000011E
//#define CPU_FEATURE_UNUSED              0x0000011F

/*
 * These defines are for CPUID EAX = 01H, features from EDX.
 */
#define CPU_FEATURE_FPU                 0x00000200
#define CPU_FEATURE_VME                 0x00000201
#define CPU_FEATURE_DE                  0x00000202
#define CPU_FEATURE_PSE                 0x00000203
#define CPU_FEATURE_TSC                 0x00000204
#define CPU_FEATURE_MSR                 0x00000205
#define CPU_FEATURE_PAE                 0x00000206
#define CPU_FEATURE_MCE                 0x00000207
#define CPU_FEATURE_CX8                 0x00000208
#define CPU_FEATURE_APIC                0x00000209
//#define CPU_FEATURE_RESERVED            0x0000020A
#define CPU_FEATURE_SEP                 0x0000020B
#define CPU_FEATURE_MTRR                0x0000020C
#define CPU_FEATURE_PGE                 0x0000020D
#define CPU_FEATURE_MCA                 0x0000020E
#define CPU_FEATURE_CMOV                0x0000020F
#define CPU_FEATURE_PAT                 0x00000210
#define CPU_FEATURE_PSE_36              0x00000211
#define CPU_FEATURE_PSN                 0x00000212
#define CPU_FEATURE_CLFSH               0x00000213
//#define CPU_FEATURE_RESERVED            0x00000214
#define CPU_FEATURE_DS                  0x00000215
#define CPU_FEATURE_ACPI                0x00000216
#define CPU_FEATURE_MMX                 0x00000217
#define CPU_FEATURE_FXSR                0x00000218
#define CPU_FEATURE_SSE                 0x00000219
#define CPU_FEATURE_SSE2                0x0000021A
#define CPU_FEATURE_SS                  0x0000021B
#define CPU_FEATURE_HTT                 0x0000021C
#define CPU_FEATURE_TM                  0x0000021D
//#define CPU_FEATURE_RESERVED            0x0000021E
#define CPU_FEATURE_PBE                 0x0000021F

/*
 * @brief Initializes the informations about cpu.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully.
 *  ERROR_UNSUPPORTED - if the cpu does not support cpuid instruction.
 */
size_t CPUID_Init();

/*
 * @brief Check if the processor has a certain feature.
 *
 * @param a_featureId The feature id
 *
 * @return
 *  ERROR_SUCCESS - if the cpu has this feature.
 *  ERROR_UNSUPPORTED - if the cpu does not have this feature.
 *  ERROR_UNINITIALIZED - if the informations abous cpu wasn't initialized.
 *  ERROR_INVALID_PARAMETER - if a_featureId is not defined.
 */
size_t CPUID_HasFeature(size_t a_featureId);

/*
 * @brief Get the vendor name from CPUID EAX = 00H
 *
 * @param a_vendorName In this parameter will be stored the pointer to the
 *                     vendor name. This must not be freed because it's a
 *                     static string.
 *
 * @return
 *  ERROR_SUCCESS - if the function ends successfully.
 *  ERROR_NULL_POINTER - if a_vendorName is null.
 *  ERROR_UNINITIALIZED - if before calling this function was not called
 *      CPUID_Init() function.
 */
size_t CPUID_GetVendorName(const char **a_vendorName);

#endif
