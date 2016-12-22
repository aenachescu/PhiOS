# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND VIDEO_C_SOURCES "${MODULE_PATH}/src/vga/text_mode.c")

# Set include paths
list(APPEND VIDEO_INCLUDE_PATH "${MODULE_PATH}/include")

# Add include paths
include_directories(${VIDEO_INCLUDE_PATH})

# Add library
add_library(VideoDriver ${VIDEO_C_SOURCES})
list(APPEND VIDEO_STATIC_LIBS "VideoDriver")

# Update parent scope
set(DRIVERS_INCLUDE_PATH "${DRIVERS_INCLUDE_PATH};${VIDEO_INCLUDE_PATH}" PARENT_SCOPE)
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${VIDEO_STATIC_LIBS}" PARENT_SCOPE)
