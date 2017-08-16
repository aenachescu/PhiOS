# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

if (DEFINED ARCH)
    if (ARCH STREQUAL "x86_32")

        # add C sources for x86_32
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/cpuid.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/init32.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/asm_io.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/gdt32.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/idt32.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/handlers32.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/pit.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/paging/ia32.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/cpu.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/pic.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/tss32.c")
        list(APPEND ARCH_C_SOURCES "${MODULE_PATH}/drng.c")

        # add asm sources for x86_32
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/registers.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/descriptors32.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/interrupts32.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/cpuid.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/usermode.s")
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/boot32.s")

    elseif (ARCH STREQUAL "x86_64")
        # TODO: add files for 64-bit arch
        list(APPEND ARCH_ASM_SOURCES "${MODULE_PATH}/boot64.s")
    else ()
        message(FATAL_ERROR "Architecture not supported")
    endif()
else ()
    message(FATAL_ERROR "Architecture is not defined")
endif()

# Update parent scope
set(C_SOURCES "${C_SOURCES};${ARCH_C_SOURCES}" PARENT_SCOPE)
set(ASM_SOURCES "${ASM_SOURCES};${ARCH_ASM_SOURCES}" PARENT_SCOPE)
