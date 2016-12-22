# This is a test module
# It should be used as an example for adding new modules

# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND TEST_C_SOURCES "${MODULE_PATH}/util_test.c")

# Add library
list(APPEND TEST_STATIC_LIBS "UtilTest")
add_library(UtilTest "${MODULE_PATH}/test_lib_util.c")

# Update parent scope
set(UTIL_C_SOURCES "${DRIVES_C_SOURCES};${TEST_C_SOURCES}" PARENT_SCOPE)
set(UTIL_STATIC_LIBS "${UTIL_STATIC_LIBS};${TEST_STATIC_LIBS}" PARENT_SCOPE)
