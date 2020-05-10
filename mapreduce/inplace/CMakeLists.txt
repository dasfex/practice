cmake_minimum_required(VERSION 3.15)
project(mr)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.65.1 COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(mr main.cpp)
add_executable(mapScript map.cpp)
add_executable(reduceScript reduce.cpp)

target_link_libraries(mr ${Boost_LIBRARIES})
