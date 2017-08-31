# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND VIDEO_C_SOURCES "${MODULE_PATH}/src/vga/text_mode.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add library
add_library(VideoDriver ${VIDEO_C_SOURCES})
list(APPEND VIDEO_STATIC_LIBS "VideoDriver")

# Update parent scope
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${VIDEO_STATIC_LIBS}" PARENT_SCOPE)
