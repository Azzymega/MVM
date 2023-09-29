cmake_minimum_required(VERSION 3.14)
project(testvm)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wdeprecated -fstandalone-debug")
file(GLOB mainSrc
     "source/*.hpp"
     "source/*.cpp"
)
file(GLOB jzipSrc
     "source/jar/*.hpp"
     "source/jar/*.cpp"
)
add_library(vm SHARED ${mainSrc})
add_library(jar SHARED ${jzipSrc})
add_executable(test test/mainTest.cpp)
add_executable(jarTest test/jarTest.cpp)
target_link_libraries(jar PRIVATE vm)
target_link_libraries(test PRIVATE vm)
target_link_libraries(test PRIVATE jar)
target_link_libraries(jarTest PRIVATE vm)
target_link_libraries(jarTest PRIVATE jar)
add_subdirectory(source/runtime)