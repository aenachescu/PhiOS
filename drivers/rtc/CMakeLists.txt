# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND RTC_C_SOURCES "${MODULE_PATH}/src/rtc.c")

# Add include paths
include_directories(${INCLUDE_PATH})

# Add library
add_library(RtcDriver ${RTC_C_SOURCES})
list(APPEND RTC_STATIC_LIBS "RtcDriver")

# Update parent scope
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${RTC_STATIC_LIBS}" PARENT_SCOPE)
