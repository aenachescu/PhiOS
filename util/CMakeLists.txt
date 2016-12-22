# Get a list of all directories
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Set temporary variables
list(APPEND UTIL_C_SOURCES "")
list(APPEND UTIL_ASM_SOURCES "")
list(APPEND UTIL_STATIC_LIBS "")
list(APPEND UTIL_INCLUDE_PATH "")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add subdirectories
foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach()

# Update parent scope
set(C_SOURCES "${C_SOURCES};${UTIL_C_SOURCES}" PARENT_SCOPE)
set(ASM_SOURCES "${ASM_SOURCES};${UTIL_ASM_SOURCES}" PARENT_SCOPE)
set(STATIC_LIBS "${STATIC_LIBS};${UTIL_STATIC_LIBS}" PARENT_SCOPE)
set(INCLUDE_PATH "${INCLUDE_PATH};${UTIL_INCLUDE_PATH}" PARENT_SCOPE)
