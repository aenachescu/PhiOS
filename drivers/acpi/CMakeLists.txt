# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND ACPI_C_SOURCES "${MODULE_PATH}/src/acpi.c")
list(APPEND ACPI_C_SOURCES "${MODULE_PATH}/src/SDTHeader.c")
list(APPEND ACPI_C_SOURCES "${MODULE_PATH}/src/acpi_rsdt.c")
list(APPEND ACPI_C_SOURCES "${MODULE_PATH}/src/acpi_xsdt.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add library
add_library(AcpiDriver ${ACPI_C_SOURCES})
list(APPEND ACPI_STATIC_LIBS "AcpiDriver")

set(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -DSTATIC_LINKAGE")

# Update parent scope
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${ACPI_STATIC_LIBS}" PARENT_SCOPE)