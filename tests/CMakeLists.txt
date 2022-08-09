set(BINARY tests)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC broadcaster_lib utils_lib gtest)

target_compile_options(${BINARY} PRIVATE -Wall -Wextra -Wpedantic -Werror)
