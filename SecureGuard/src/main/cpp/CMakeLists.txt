cmake_minimum_required(VERSION 3.22.1)

project("secureguard")

file(GLOB_RECURSE SRC_FILES
        jni/*.c
        core/*.c
        common/*.c
        pcapd/*.c
)

add_library(
        secureguard
        SHARED
        ${SRC_FILES}
)

find_library(
        log-lib
        log
)

target_include_directories(
        secureguard
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
        secureguard
        ${log-lib}
)
