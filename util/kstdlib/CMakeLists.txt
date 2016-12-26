# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kstring.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kstdio.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kstdlib.c")

# Add include paths
list(APPEND KSTDLIB_INCLUDE_PATH "${MODULE_PATH}/include")
list(APPEND KSTDLIB_INCLUDE_PATH "${PRJ_PATH}/drivers/video/include")
include_directories(${KSTDLIB_INCLUDE_PATH})

# Add library
list(APPEND KSTDLIB_STATIC_LIBS "KstdlibTest")
add_library(KstdlibTest ${KSTDLIB_C_SOURCES})

# Update parent scope
set(UTIL_STATIC_LIBS "${UTIL_STATIC_LIBS};${KSTDLIB_STATIC_LIBS}" PARENT_SCOPE)
set(UTIL_INCLUDE_PATH "${UTIL_INCLUDE_PATH};${KSTDLIB_INCLUDE_PATH}" PARENT_SCOPE)
