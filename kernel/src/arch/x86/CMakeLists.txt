# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C and ASM sources
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/asm_io.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/gdt32.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/idt32.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/handlers32.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/pit.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/paging/ia32.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/cpu.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/pic.c")
list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/tss32.c")

if (DEFINED ARCH)
    if (ARCH STREQUAL "x86_32")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/cpuid.c")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/descriptors32.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/interrupts32.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/cpuid.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/usermode.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/multiboot.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/boot.s")
    elseif (ARCH STREQUAL "x86_64")
        # TODO: add files for 64-bit arch
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/idt64.c")

        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/descriptors32.s")
        #list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/interrupts32.s")
        #list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/cpuid.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/usermode.s")

        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/multiboot.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/boot.s")
    else ()
        message(FATAL_ERROR "Architecture not supported")
    endif()
else ()
    list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/descriptors32.s")
    #list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/interrupts32.s")
    #list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/cpuid.s")
    list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/usermode.s")

    list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/multiboot.s")
    list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/boot.s")
endif()

# Update parent scope
set(C_SOURCES "${C_SOURCES};${ARCH_C_SOURCES}" PARENT_SCOPE)
set(ASM_SOURCES "${ASM_SOURCES};${ARCH_ASM_SOURCES}" PARENT_SCOPE)
