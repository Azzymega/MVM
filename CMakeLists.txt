cmake_minimum_required(VERSION 3.14)
project(mvm)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra -Wdeprecated -fstandalone-debug -O3")
file(GLOB mainSrc
     "source/*.hpp"
     "source/*.cpp"
)
file(GLOB jzipSrc
     "source/jar/*.hpp"
     "source/jar/*.cpp"
)
file(GLOB entryPointSource
     "source/entryPoint/*.hpp"
     "source/entryPoint/*.cpp"
)
file(GLOB source
     "source/native/*.hpp"
     "source/native/*.cpp"
)
file(GLOB systemSource
     "source/native/System/*.hpp"
     "source/native/System/*.cpp"
)
add_library(Native SHARED ${source})
add_library(System SHARED ${systemSource})
add_library(vm SHARED ${mainSrc})
add_library(jar SHARED ${jzipSrc})
add_executable(test test/mainTest.cpp)
add_executable(jarTest test/jarTest.cpp)
add_executable(runtimeTest test/runtimeLoadTest.cpp)
add_executable(endTest test/endTest.cpp)
add_executable(java ${entryPointSource})
target_link_libraries(vm PUBLIC Native)
target_link_libraries(jar PRIVATE vm)
target_link_libraries(test PRIVATE vm)
target_link_libraries(test PRIVATE jar)
target_link_libraries(jarTest PRIVATE vm)
target_link_libraries(jarTest PRIVATE jar)
target_link_libraries(runtimeTest PRIVATE vm)
target_link_libraries(runtimeTest PRIVATE jar)
target_link_libraries(java PRIVATE vm)
target_link_libraries(java PRIVATE jar)
target_link_libraries(endTest PRIVATE vm)
target_link_libraries(endTest PRIVATE jar)
add_subdirectory(source/runtime)