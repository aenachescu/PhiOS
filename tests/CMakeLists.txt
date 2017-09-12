cmake_minimum_required(VERSION 3.5.1)

project (PhiOS C)
enable_language(C)

set(PRJ_PATH ${CMAKE_SOURCE_DIR})

# set include directories
include_directories("${PRJ_PATH}/../")
include_directories("${PRJ_PATH}/../CUT/include")

# set definitions
add_definitions(-DUNIT_TEST)

if (DEFINED ARCH)
    if (ARCH STREQUAL "x86_32")
        add_definitions(-DPhiOS_ARCH_x86_32)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
    elseif (ARCH STREQUAL "x86_64")
        add_definitions(-DPhiOS_ARCH_x86_64)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
    else()
        message(FATAL_ERROR "Architecture not supported")
    endif()
else()
    message(FATAL_ERROR "Architecture is not defined")
endif()

# set path to CUT library
link_directories("${PRJ_PATH}/../CUT/bin")

# set C flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wno-format -fprofile-arcs -ftest-coverage")

# PAA test
list(APPEND PAA_SOURCES "${PRJ_PATH}/../kernel/src/memory/paa.c")
list(APPEND PAA_SOURCES "src/test_paa.c")
add_executable(test_paa ${PAA_SOURCES})
target_link_libraries(test_paa cut)

# BitmapPMA test
list(APPEND BITMAP_SOURCES "${PRJ_PATH}/../kernel/src/memory/bitmap_pma.c")
list(APPEND BITMAP_SOURCES "src/test_bitmap.c")
list(APPEND BITMAP_SOURCES "mock/mock_paa.c")
add_executable(test_bitmap ${BITMAP_SOURCES})
target_link_libraries(test_bitmap cut)

# kstdlib test
list (APPEND KSTDLIB_SOURCES "${PRJ_PATH}/../util/kstdlib/src/ksnprintf_helper.c")
list (APPEND KSTDLIB_SOURCES "${PRJ_PATH}/../util/kstdlib/src/ksnprintf.c")
list (APPEND KSTDLIB_SOURCES "${PRJ_PATH}/../util/kstdlib/src/kvsnprintf.c")
list (APPEND KSTDLIB_SOURCES "${PRJ_PATH}/../util/kstdlib/src/kstdlib.c")
list (APPEND KSTDLIB_SOURCES "${PRJ_PATH}/../util/kstdlib/src/kstring.c")

list (APPEND KSTDLIB_SOURCES "src/kstdlib/test_ksnprintf_helper.c")
list (APPEND KSTDLIB_SOURCES "src/kstdlib/test_ksnprintf.c")
list (APPEND KSTDLIB_SOURCES "src/kstdlib/test_kvsnprintf.c")
list (APPEND KSTDLIB_SOURCES "src/kstdlib/test_kstring.c")
list (APPEND KSTDLIB_SOURCES "src/kstdlib/test_kstdlib.c")

add_executable(test_kstdlib ${KSTDLIB_SOURCES})
target_link_libraries(test_kstdlib cut)
