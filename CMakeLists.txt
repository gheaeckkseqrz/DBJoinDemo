cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 17)

enable_testing()
include_directories(include)

add_subdirectory(src)
add_subdirectory(test)


