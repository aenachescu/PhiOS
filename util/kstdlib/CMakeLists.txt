# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add sources
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kstring.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kstdio.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kstdlib.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/ksnprintf_helper.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/ksnprintf.c")
list(APPEND KSTDLIB_C_SOURCES "${MODULE_PATH}/src/kvsnprintf.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add library
list(APPEND KSTDLIB_STATIC_LIBS "KstdlibTest")
add_library(KstdlibTest ${KSTDLIB_C_SOURCES})

# Update parent scope
set(UTIL_STATIC_LIBS "${UTIL_STATIC_LIBS};${KSTDLIB_STATIC_LIBS}" PARENT_SCOPE)
