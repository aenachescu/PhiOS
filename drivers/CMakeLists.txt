# Get a list of all directories
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

# Set temporary variables
list(APPEND DRIVERS_C_SOURCES "")
list(APPEND DRIVERS_ASM_SOURCES "")
list(APPEND DRIVERS_STATIC_LIBS "")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add subdirectories
foreach(subdir ${SUBDIRS})
    add_subdirectory(${subdir})
endforeach()

# Update parent scope
set(C_SOURCES "${C_SOURCES};${DRIVERS_C_SOURCES}" PARENT_SCOPE)
set(ASM_SOURCES "${ASM_SOURCES};${DRIVERS_ASM_SOURCES}" PARENT_SCOPE)
set(STATIC_LIBS "${STATIC_LIBS};${DRIVERS_STATIC_LIBS}" PARENT_SCOPE)
