# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND QEMU_C_SOURCES "${MODULE_PATH}/power.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Update parent scope
set(KERNEL_C_SOURCES "${KERNEL_C_SOURCES};${QEMU_C_SOURCES}" PARENT_SCOPE)
