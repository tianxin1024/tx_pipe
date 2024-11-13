cmake_minimum_required(VERSION 3.10)

add_library(log_file_writer STATIC log_file_writer.cpp)
set_property(TARGET log_file_writer PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(log_file_writer PUBLIC stdc++fs)

add_library(logger STATIC logger.cpp)
set_property(TARGET logger PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(logger PUBLIC log_file_writer)

