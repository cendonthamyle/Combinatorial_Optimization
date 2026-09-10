cmake_minimum_required(VERSION 3.16)

project(CombinatorialOptimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(tsplib_reader
    common/src/Data.cpp
)

target_include_directories(tsplib_reader PUBLIC
    common/include
)

target_compile_options(tsplib_reader PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall;-Wextra;-Wpedantic>
)
