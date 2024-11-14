cmake_minimum_required(VERSION 3.10)

add_library(size STATIC size.cpp)
set_property(TARGET size PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(point STATIC point.cpp)
set_property(TARGET point PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(line STATIC line.cpp)
set_property(TARGET line PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(line PUBLIC point)

add_library(rect STATIC rect.cpp)
set_property(TARGET rect PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(rect PUBLIC point size)

add_library(polygon STATIC polygon.cpp)
set_property(TARGET polygon PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(polygon PUBLIC point)


