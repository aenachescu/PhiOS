# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND KEYBOARD_C_SOURCES "${MODULE_PATH}/src/keyboard.c")

# Set include paths
list(APPEND KEYBOARD_INCLUDE_PATH "${MODULE_PATH}/include")

# Add include paths
include_directories(${KEYBOARD_INCLUDE_PATH})

# Add library
add_library(KeyboardDriver ${KEYBOARD_C_SOURCES})
list(APPEND KEYBOARD_STATIC_LIBS "KeyboardDriver")

# Update parent scope
set(DRIVERS_INCLUDE_PATH "${DRIVERS_INCLUDE_PATH};${KEYBOARD_INCLUDE_PATH}" PARENT_SCOPE)
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${KEYBOARD_STATIC_LIBS}" PARENT_SCOPE)
