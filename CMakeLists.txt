## COPYRIGHT (C) HARRY CLARK 2025
## STANDALONE REFERENCE UTILITY FOR SIMULATING THE MOTOROLA 68851 MMU

cmake_minimum_required(VERSION 3.20)

project(noodle)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NOODLE_TEST "NOODLE: USE TEST SUITE" OFF)

find_package(fmt REQUIRED)

add_executable(noodle
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc"
)

target_include_directories(noodle PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/inc"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/noodle/src"
)

target_link_libraries(noodle PRIVATE fmt::fmt)

target_compile_options(noodle PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
    -std=c++17
)

if(NOODLE_TEST AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    file(GLOB TEST_SOURCES "tests/*.cc" "tests/**/*.cc")

    add_executable(noodle_tests ${TEST_SOURCES})
    target_include_directories(noodle_tests PUBLIC inc tests)
    target_link_libraries(noodle_tests PRIVATE fmt::fmt)
    target_compile_options(noodle_tests PRIVATE -Wall -Wextra -Wno-unused-parameter -std=c++17)
    
    enable_testing()
    add_test(NAME noodle_tests COMMAND noodle_tests)
    message(STATUS "USING NOODLE TEST")
endif()
