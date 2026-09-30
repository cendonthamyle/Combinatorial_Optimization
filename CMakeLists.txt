cmake_minimum_required(VERSION 3.16)

project(CombinatorialOptimization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# LEITOR DE INSTÂNCIAS TSPLIB
# Biblioteca compartilhada pelos diferentes algoritmos do projeto
add_library(tsplib_reader
    common/src/Data.cpp
)

target_include_directories(tsplib_reader PUBLIC
    common/include
)

target_compile_options(tsplib_reader PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall;-Wextra;-Wpedantic>
)

# COMPONENTES CENTRAIS DO TSP
add_library(tsp_core
    01_tsp/src/Solution.cpp
    01_tsp/src/Construction.cpp
    01_tsp/src/LocalSearch.cpp
    01_tsp/src/Perturbation.cpp
    01_tsp/src/ILS.cpp
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

# EXECUTÁVEL PRINCIPAL
add_executable(tsp
    01_tsp/src/main.cpp
)

target_link_libraries(tsp PRIVATE
    tsp_core
)

target_compile_options(tsp PRIVATE
    $<$<CXX_COMPILER_ID:AppleClang,Clang,GNU>:-Wall;-Wextra;-Wpedantic>
)