cmake_minimum_required(VERSION 3.5.1)

project (PhiOS C)
enable_language(C)

list(APPEND SOURCES "bitmap_pma.h")
list(APPEND SOURCES "main.c")

add_executable(tests ${SOURCES})
target_link_libraries(tests cut)
