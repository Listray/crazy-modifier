cmake_minimum_required(VERSION 3.0)

project(modifier)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB DEBUG_MODULE_SOURCE *.cpp)

add_executable(modifier ${DEBUG_MODULE_SOURCE})
target_link_libraries(modifier jsoncpp)