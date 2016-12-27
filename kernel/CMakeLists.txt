# Get a list of all directories
SUBDIRLIST(SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Set temporary variables
list(APPEND KERNEL_C_SOURCES "")
list(APPEND KERNEL_ASM_SOURCES "")
list(APPEND KERNEL_STATIC_LIBS "")
list(APPEND KERNEL_INCLUDE_PATH "")

list(APPEND INCLUDE_PATH "${PRJ_PATH}/util/kstdlib/include")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add subdirectories
foreach(subdir ${SUBDIRS})
    if (NOT (${subdir} STREQUAL "arch"))
        add_subdirectory(${subdir})
    endif()
endforeach()

# Update parent scope
set(C_SOURCES "${C_SOURCES};${KERNEL_C_SOURCES}" PARENT_SCOPE)
set(ASM_SOURCES "${ASM_SOURCES};${KERNEL_ASM_SOURCES}" PARENT_SCOPE)
set(STATIC_LIBS "${STATIC_LIBS};${KERNEL_STATIC_LIBS}" PARENT_SCOPE)
set(INCLUDE_PATH "${INCLUDE_PATH};${KERNEL_INCLUDE_PATH}" PARENT_SCOPE)
