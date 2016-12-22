# This is a test module
# It should be used as an example for adding new modules

# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND TEST_C_SOURCES "${MODULE_PATH}/drivers_test.c")

# Add library
list(APPEND TEST_STATIC_LIBS "DriversTest")
add_library(DriversTest "${MODULE_PATH}/test_lib_test.c")

# Update parent scope
set(DRIVERS_C_SOURCES "${DRIVES_C_SOURCES};${TEST_C_SOURCES}" PARENT_SCOPE)
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${TEST_STATIC_LIBS}" PARENT_SCOPE)
