cmake_minimum_required(VERSION 3.14)
project(testvm)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wdeprecated -fstandalone-debug")
file(GLOB_RECURSE src
     "source/*.hpp"
     "source/*.cpp"
)
add_library(vm SHARED ${src})
add_executable(test test/mainTest.cpp)
target_link_libraries(test PRIVATE vm)