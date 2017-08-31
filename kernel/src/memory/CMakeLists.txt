# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND MEMORY_C_SOURCES "${MODULE_PATH}/paa.c")
list(APPEND MEMORY_C_SOURCES "${MODULE_PATH}/bitmap_pma.c")
list(APPEND MEMORY_C_SOURCES "${MODULE_PATH}/pmm.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Update parent scope
set(KERNEL_C_SOURCES "${KERNEL_C_SOURCES};${MEMORY_C_SOURCES}" PARENT_SCOPE)
