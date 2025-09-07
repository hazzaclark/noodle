## COPYRIGHT (C) HARRY CLARK 2025
## STANDALONE REFERENCE UTILITY FOR SIMULATING THE MOTOROLA 68851 MMU

cmake_minimum_required(VERSION 3.20)

project(noodle)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(fmt REQUIRED)

add_executable(noodle
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc"
)

target_include_directories(noodle PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_link_libraries(noodle PRIVATE fmt::fmt)

target_compile_options(noodle PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    -std=c++17
)