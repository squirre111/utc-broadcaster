set(BINARY broadcaster)

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY} ${SOURCES})

target_link_libraries(${BINARY} utils_lib)

target_compile_options(${BINARY} PRIVATE -Wall -Wextra -Wpedantic -Werror)


add_library(${BINARY}_lib STATIC ${SOURCES})

target_link_libraries(${BINARY}_lib utils_lib)

