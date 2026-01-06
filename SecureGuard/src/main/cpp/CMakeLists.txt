cmake_minimum_required(VERSION 3.22.1)

project(secureguard LANGUAGES C CXX)

# --------------------------------------------------
# Collect all native source files
# --------------------------------------------------
file(GLOB_RECURSE SECUREGUARD_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/jni/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/jni/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/core/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp
)

# --------------------------------------------------
# Create shared library
# --------------------------------------------------
add_library(
        secureguard
        SHARED
        ${SECUREGUARD_SOURCES}
)

# --------------------------------------------------
# Android log library
# --------------------------------------------------
find_library(
        log-lib
        log
)

# --------------------------------------------------
# Include directories
# --------------------------------------------------
target_include_directories(
        secureguard
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/common
)

# --------------------------------------------------
# Compiler flags (recommended for security libs)
# --------------------------------------------------
target_compile_options(secureguard PRIVATE
        -fvisibility=hidden
        -fno-exceptions
        -fno-rtti
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
        secureguard
        ${log-lib}
)
