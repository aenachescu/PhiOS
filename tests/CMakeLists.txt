cmake_minimum_required(VERSION 3.5.1)

project (PhiOS C)
enable_language(C)

set(PRJ_PATH ${CMAKE_SOURCE_DIR})

# set include directories
include_directories("${PRJ_PATH}/../")
include_directories("${PRJ_PATH}/../include")
include_directories("${PRJ_PATH}/../CUT/include")

# set definitions
add_definitions(-DPhiOS_ARCH_x86_64 -DUNIT_TEST)

# set path to CUT library
link_directories("${PRJ_PATH}/../CUT/bin")

# set C flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")

# PAA test
list(APPEND PAA_SOURCES "${PRJ_PATH}/../kernel/src/memory/paa.c")
list(APPEND PAA_SOURCES "src/test_paa.c")
add_executable(test_paa ${PAA_SOURCES})
target_link_libraries(test_paa cut)

# BitmapPMA test
list(APPEND BITMAP_SOURCES "${PRJ_PATH}/../kernel/src/memory/bitmap_pma.c")
list(APPEND BITMAP_SOURCES "src/test_bitmap.c")
add_executable(test_bitmap ${BITMAP_SOURCES})
target_link_libraries(test_bitmap cut)
