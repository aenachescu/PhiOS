# Set current path
set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

# Add C sources
list(APPEND RTC_C_SOURCES "${MODULE_PATH}/src/rtc.c")

# Set include paths
list(APPEND RTC_INCLUDE_PATH "${MODULE_PATH}/include")

# Add include paths
include_directories(${RTC_INCLUDE_PATH})

# Add library
add_library(RtcDriver ${RTC_C_SOURCES})
list(APPEND RTC_STATIC_LIBS "RtcDriver")

# Update parent scope
set(DRIVERS_INCLUDE_PATH "${DRIVERS_INCLUDE_PATH};${RTC_INCLUDE_PATH}" PARENT_SCOPE)
set(DRIVERS_STATIC_LIBS "${DRIVERS_STATIC_LIBS};${RTC_STATIC_LIBS}" PARENT_SCOPE)
