# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND MEMORY_C_SOURCES "${MODULE_PATH}/paa.c")
list(APPEND MEMORY_C_SOURCES "${MODULE_PATH}/bitmap_pma.c")

# Add include paths
list(APPEND MEMORY_INCLUDE_PATH "${PRJ_PATH}/kernel/include/memory")
include_directories(${MEMORY_INCLUDE_PATH})

# Update parent scope
set(KERNEL_C_SOURCES "${KERNEL_C_SOURCES};${MEMORY_C_SOURCES}" PARENT_SCOPE)
set(KERNEL_INCLUDE_PATH "${KERNEL_INCLUDE_PATH};${MEMORY_INCLUDE_PATH}" PARENT_SCOPE)
