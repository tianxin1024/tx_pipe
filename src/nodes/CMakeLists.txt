cmake_minimum_required(VERSION 3.10)

add_library(meta_subscriber STATIC meta_subscriber.cpp)
set_property(TARGET meta_subscriber PROPERTY POSITION_INDEPENDENT_CODE ON)

add_library(meta_publisher STATIC meta_publisher.cpp)
set_property(TARGET meta_publisher PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(meta_publisher PUBLIC meta_subscriber)

add_library(node STATIC node.cpp)
set_property(TARGET node PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(node PUBLIC meta_subscriber logger meta_publisher control_meta frame_meta)

add_library(des_node STATIC des_node.cpp)
set_property(TARGET des_node PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(des_node PUBLIC node)

add_library(src_node STATIC src_node.cpp)
set_property(TARGET src_node PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(src_node PUBLIC node)

add_library(file_src_node STATIC file_src_node.cpp)
set_property(TARGET file_src_node PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(file_src_node PUBLIC src_node opencv_core opencv_imgproc opencv_highgui ${DEPEND_LIBS})
