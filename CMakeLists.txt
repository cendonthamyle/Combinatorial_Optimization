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

enable_testing()

add_executable(test_data
    common/tests/test_data.cpp
)

target_link_libraries(test_data PRIVATE
    tsplib_reader
)

add_test(
    NAME data_reader_tests
    COMMAND test_data
)

set_tests_properties(data_reader_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_library(tsp_core
    01_tsp/src/Solution.cpp
)

target_include_directories(tsp_core PUBLIC
    01_tsp/include
)

target_link_libraries(tsp_core PUBLIC
    tsplib_reader
)

target_compile_options(tsp_core PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall;-Wextra;-Wpedantic>
)

add_executable(test_solution
    01_tsp/tests/test_solution.cpp
)

target_link_libraries(test_solution PRIVATE
    tsp_core
)

add_test(
    NAME solution_tests
    COMMAND test_solution
)

set_tests_properties(solution_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)