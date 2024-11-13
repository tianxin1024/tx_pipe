cmake_minimum_required(VERSION 3.10)

add_library(meta STATIC meta.cpp)
set_property(TARGET meta PROPERTY POSITION_INDEPENDENT_CODE ON)

