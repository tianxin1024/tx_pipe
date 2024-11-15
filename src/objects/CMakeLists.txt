cmake_minimum_required(VERSION 3.10)

add_subdirectory(shapes)

add_library(meta STATIC meta.cpp)
set_property(TARGET meta PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(frame_meta STATIC frame_meta.cpp)
set_property(TARGET frame_meta PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(control_meta STATIC control_meta.cpp)
set_property(TARGET control_meta PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(control_meta PUBLIC meta)

add_library(frame_face_target STATIC frame_face_target.cpp)
set_property(TARGET frame_face_target PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(frame_face_target PUBLIC meta rect)
