cmake_minimum_required(VERSION 3.5.1)

project (PhiOS C ASM)

if (DEFINED VERBOSE)
    if (VERBOSE STREQUAL "on")
        set(CMAKE_VERBOSE_MAKEFILE ON)
    else ()
        set(CMAKE_VERBOSE_MAKEFILE OFF)
    endif ()
else ()
    set(CMAKE_VERBOSE_MAKEFILE OFF)
endif ()

# Enable languages
enable_language(C)
enable_language(ASM)
set(can_use_assembler TRUE)

# Define macros
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# Set paths
set(PRJ_PATH ${CMAKE_SOURCE_DIR})

# Set compilers for target architecture
if (DEFINED ARCH)
    if (ARCH STREQUAL "x86_32")
        set(CMAKE_ASM_COMPILER "i686-elf-as")
        set(CMAKE_C_COMPILER "i686-elf-gcc")
        set(CMAKE_AR "i686-elf-ar")
        set(CMAKE_LINKER "i686-elf-ld")
        set(LINKER_FILE "${PRJ_PATH}/kernel/src/arch/x86/linker32.ld")
        set(ARCH_PATH "kernel/src/arch/x86")
        set(CMAKE_C_FLAGS " -DPhiOS_ARCH_x86_32")
    elseif (ARCH STREQUAL "x86_64")
        set(C_32BIT_COMPILER "i686-elf-gcc")
        set(ASM_32BIT_COMPILER "i686-elf-as")

        set(CMAKE_ASM_COMPILER "x86_64-elf-as")
        set(CMAKE_C_COMPILER "x86_64-elf-gcc")
        set(CMAKE_AR "x86_64-elf-ar")
        set(CMAKE_LINKER "x86_64-elf-ld")
        set(LINKER_FILE "${PRJ_PATH}/kernel/src/arch/x86/linker64.ld")
        set(ARCH_PATH "kernel/src/arch/x86")
        set(CMAKE_C_FLAGS " -DPhiOS_ARCH_x86_64")
    else ()
        message(FATAL_ERROR "Architecture not supported")
    endif()
else ()
    message(FATAL_ERROR "Architecture is not defined")
endif()

# Set flags
set(CMAKE_ASM_FLAGS "")
set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -std=gnu11 -ffreestanding -Wall -Wextra -fpie -O2")
set(CMAKE_LINKER_FLAGS "-T ${LINKER_FILE} -O2 -nostdlib -z max-page-size=0x1000")

# Set linker rule
set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_LINKER> ${CMAKE_LINKER_FLAGS} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

# Set sources
list(APPEND BOOTSTRAP_SOURCES "")
list(APPEND C_SOURCES "")
list(APPEND ASM_SOURCES "")
list(APPEND STATIC_LIBS "")

# Set include paths
list(APPEND INCLUDE_PATH "${PRJ_PATH}/include")
list(APPEND INCLUDE_PATH "${PRJ_PATH}/kernel/include")
list(APPEND INCLUDE_PATH "${PRJ_PATH}")

# Add subdirectories
add_subdirectory("${PRJ_PATH}/kernel")
add_subdirectory("${PRJ_PATH}/${ARCH_PATH}")
add_subdirectory("${PRJ_PATH}/drivers")
add_subdirectory("${PRJ_PATH}/util")

if (DEFINED ARCH)
    if (ARCH STREQUAL "x86_32")
        list(APPEND STATIC_LIBS "${PRJ_PATH}/build/linux-x86_64/gcc-x86_32/lib/gcc/i686-elf/4.9.1/libgcc.a")
        list(APPEND C_SOURCES "${PRJ_PATH}/kernel/src/phi_main.c")
        list(APPEND C_SOURCES "${PRJ_PATH}/kernel/src/errors.c")
    endif()
endif()


# Add include paths
include_directories(${INCLUDE_PATH})

# Set custom commands
add_custom_command(
    OUTPUT
        phios.iso
    COMMAND
        mkdir -p isodir/boot/grub &&
        cp phios.elf isodir/boot/phios.elf &&
        cp ${PRJ_PATH}/grub.cfg isodir/boot/grub/grub.cfg &&
        grub-mkrescue -d /usr/lib/grub/i386-pc -o phios.iso isodir
    DEPENDS
        phios.elf
    )

# Set custom targets
add_custom_target(iso DEPENDS phios.iso)
add_custom_target(bootstrap DEPENDS ${PRJ_PATH}/bin/x86_64/boot.o)

# Add OS image
add_executable(phios.elf ${C_SOURCES} ${ASM_SOURCES})
target_link_libraries(phios.elf ${STATIC_LIBS})