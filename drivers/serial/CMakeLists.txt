# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND SERIAL_C_SOURCES "${MODULE_PATH}/src/serial.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add library
add_library(SerialDriver ${SERIAL_C_SOURCES})
list(APPEND SERIAL_STATIC_LIBS "SerialDriver")

# Update parent scope
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${SERIAL_STATIC_LIBS}" PARENT_SCOPE)
