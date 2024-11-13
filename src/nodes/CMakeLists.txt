cmake_minimum_required(VERSION 3.10)

add_library(meta_subscriber STATIC meta_subscriber.cpp)
set_property(TARGET meta_subscriber PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(meta_publisher STATIC meta_publisher.cpp)
set_property(TARGET meta_publisher PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(meta_publisher PUBLIC meta_subscriber)
