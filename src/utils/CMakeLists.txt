cmake_minimum_required(VERSION 3.10)

set(BINARY utils_lib)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${SOURCES})

add_library(${BINARY} STATIC ${SOURCES})

target_include_directories(${BINARY} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${BINARY} PRIVATE -Wall -Wextra -Wpedantic -Werror)
