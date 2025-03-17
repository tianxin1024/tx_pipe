cmake_minimum_required(VERSION 3.10)

# add_executable(demo sample.cpp)
# set_property(TARGET demo PROPERTY POSITION_INDEPENDENT_CODE  ON)
# target_link_libraries(demo PUBLIC file_src_node analysis_board yunet_face_detector_node 
#     sface_feature_encoder_node face_osd_node_v2 screen_des_node logger)

add_executable(rk_demo rk_sample.cpp)
set_property(TARGET rk_demo PROPERTY POSITION_INDEPENDENT_CODE  ON)
target_link_libraries(rk_demo PUBLIC rk_rtsp_src_node rk_first_yolo logger)
