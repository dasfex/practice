cmake_minimum_required(VERSION 3.21)
project(listin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER_ID "clang")

message([STATUS] " Use compiler: ${CMAKE_CXX_COMPILER_ID}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)

file(GLOB SRC_FILES *.cpp)
add_executable(test tests_memory_list.cpp)
add_executable (bench benchmarks.cpp)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(googlebenchmark)
target_link_libraries(test gtest gtest_main gmock)
target_link_libraries(bench gtest gtest_main gmock benchmark::benchmark)