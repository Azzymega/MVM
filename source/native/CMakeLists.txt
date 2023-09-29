cmake_minimum_required(VERSION 3.14)
project(nativeLib)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wdeprecated -fstandalone-debug")
file(GLOB source
     "*.hpp"
     "*.cpp"
)
add_library(Native SHARED ${source})