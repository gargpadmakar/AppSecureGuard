cmake_minimum_required(VERSION 3.22.1)

project(secureguard LANGUAGES C CXX)

# --------------------------------------------------
# Collect all native source files (UPDATED)
# --------------------------------------------------
file(GLOB_RECURSE SECUREGUARD_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/jni/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/jni/*.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/core/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/security/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/security/*.cpp
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
# Android system libraries
# --------------------------------------------------
find_library(
        log-lib
        log
)

# --------------------------------------------------
# Include directories (UPDATED)
# --------------------------------------------------
target_include_directories(
        secureguard
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/jni
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/common
        ${CMAKE_CURRENT_SOURCE_DIR}/security
)

# --------------------------------------------------
# Compiler flags (security-focused)
# --------------------------------------------------
target_compile_options(secureguard PRIVATE
        -fvisibility=hidden
        -fno-exceptions
        -fno-rtti
        -fstack-protector-strong
        -fPIE
)

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(
        secureguard
        ${log-lib}
)
