cmake_minimum_required(VERSION 3.10)

project(UtcBroadcaster)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(src/utils)
add_subdirectory(src/broadcaster)
add_subdirectory(src/reciever)
add_subdirectory(lib/googletest)
add_subdirectory(tests)
