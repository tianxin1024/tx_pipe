/*
* ## trt yolov8 sample ##
* detection/segmentation/pose_estimation using yolov8 based on tensorrt
*/
#include <iostream>
#include "utils/logger.h"
#include "utils/utils.h"
#include "nodes/file_src_node.h"
#include "nodes/infers/trt_yolov8_detector.h"
#include "nodes/screen_des_node.h"

int main() {
    SET_LOG_LEVEL(tx_utils::LogLevel::INFO);
    LOGGER_INIT();

    // create nodes for 1st pipeline
    auto file_src_0 = std::make_shared<tx_nodes::FileSrcNode>("file_src_0", 0, "../workspace/video/face2.mp4");
    auto yolo8_detector = std::make_shared<tx_nodes::TrtYolov8Detector>(
        "yolo8_detector", "../workspace/models/trt/yolov8s_v8.5.engine", "../workspace/models/coco_80classes.txt");
    auto osd_0 = std::make_shared<tx_nodes::OsdNodeV3>("osd_0", "./vp_data/font/NotoSansCJKsc-Medium.otf");
    auto screen_des_0 = std::make_shared<tx_nodes::ScreenDesNode>("screen_des_0", 0);

    return 0;
}
