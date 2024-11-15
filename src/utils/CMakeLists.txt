cmake_minimum_required(VERSION 3.10)

add_library(log_file_writer STATIC log_file_writer.cpp)
set_property(TARGET log_file_writer PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(log_file_writer PUBLIC stdc++fs)

add_library(logger STATIC logger.cpp)
set_property(TARGET logger PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(logger PUBLIC log_file_writer)

add_library(node_on_screen STATIC node_on_screen.cpp)
set_property(TARGET node_on_screen PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(node_on_screen PUBLIC des_node)

add_library(analysis_board STATIC analysis_board.cpp)
set_property(TARGET analysis_board PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(analysis_board PUBLIC node_on_screen rect)

