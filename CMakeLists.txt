project(EyeSight)

cmake_minimum_required(VERSION 2.8.7)

# Enable automoc and include current directory
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set version
set(EYESIGHT_VERSION_MAJOR 0)
set(EYESIGHT_VERSION_MINOR 0)
set(EYESIGHT_VERSION_PATCH 0)
set(EYESIGHT_VERSION_STRING "${EYESIGHT_VERSION_MAJOR}.${EYESIGHT_VERSION_MINOR}.${EYESIGHT_VERSION_PATCH}")

# Installation directories
include(MauiInstallDirs)

# Make CPack available to easy generate binary packages
include(CPack)

# Add custom uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Build flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -fvisibility-inlines-hidden -Wall -std=c++0x")

# Assume release build by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "" FORCE)
endif()

# Disable debug output for release builds
if(CMAKE_BUILD_TYPE MATCHES "^[Rr]elease$")
    add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

# Find Qt
set(QT_MIN_VERSION "5.0.0")
find_package(Qt5Widgets REQUIRED)
if(NOT Qt5Widgets_FOUND)
    message(FATAL_ERROR "Qt5Widgets module is required!")
endif()
find_package(Qt5PrintSupport REQUIRED)
if(NOT Qt5PrintSupport_FOUND)
    message(FATAL_ERROR "Qt5PrintSupport module is required!")
endif()

# Find Vibe libraries
find_package(VibeWidgets REQUIRED)
if(NOT VibeWidgets_FOUND)
    message(FATAL_ERROR "VibeWidgets module is required!")
endif()

# Subdirectories
add_subdirectory(src)
