cmake_minimum_required(VERSION 3.10)

project(tx_pipe LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fPIC -w -fdiagnostics-color=always -pthread")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CXX_HEADER_DIRS "")
set(COMMON_LIB_DIRS "")

include_directories(${CXX_HEADER_DIRS})
link_directories(${COMMON_LIB_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(example)
