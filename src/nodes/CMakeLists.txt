cmake_minimum_required(VERSION 3.10)

add_library(meta_subscriber STATIC meta_subscriber.cpp)
set_property(TARGET meta_subscriber PROPERTY POSITION_INDEPENDENT_CODE ON)

