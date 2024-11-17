cmake_minimum_required(VERSION 3.10)

add_library(face_osd_node_v2 STATIC face_osd_node_v2.cpp)
set_property(TARGET face_osd_node_v2 PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(face_osd_node_v2 PUBLIC )


