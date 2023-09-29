cmake_minimum_required (VERSION 3.14) # пока я не напишу свой компилятор jav'ы
find_package(Java REQUIRED)
include(UseJava)
project (javaSTDRuntime)
set(CMAKE_JAVA_COMPILE_FLAGS "-source" "1.7" "-target" "1.7")
file(GLOB runtimeSrc
     "*.java"
)
add_jar(Runtime ${runtimeSrc})