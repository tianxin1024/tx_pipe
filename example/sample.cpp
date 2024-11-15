#include <iostream>
#include "utils/logger.h"
#include "nodes/file_src_node.h"
#include "utils/analysis_board.h"
#include "nodes/infers/yunet_face_detector_node.h"
#include "nodes/infers/sface_feature_encoder_node.h"

/*
 * ## 1-1-1 sample ##
 * 1 video input, 1 infer task, and 1 output.
 */

using namespace tx_utils;

int main() {
    SET_LOG_INCLUDE_CODE_LOCATION(false);
    SET_LOG_INCLUDE_THREAD_ID(false);
    LOGGER_INIT();

    // create nodes
    auto file_src_0 = std::make_shared<tx_nodes::FileSrcNode>(
        "file_src_0", 0, "../../workspace/video/face.mp4", 0.6);
    auto yunet_face_detector_0 = std::make_shared<tx_nodes::YunetFaceDetectorNode>(
        "yunet_face_detector_0", "../../workspace/onnx/face_detection_yunet_2022mar.onnx");
    auto sface_face_encoder_0 = std::make_shared<tx_nodes::SfaceFeatureEncoderNode>(
        "sface_face_encoder_0", "../../workspace/onnx/face_recognition_sface_2021dec.onnx");
    // auto osd_0 = std::make_shared<vp_nodes::vp_face_osd_node_v2>("osd_0");
    // auto screen_des_0 =
    //     std::make_shared<vp_nodes::vp_screen_des_node>("screen_des_0", 0);

    // construct pipeline
    // yunet_face_detector_0->attach_to({file_src_0});
    // sface_face_encoder_0->attach_to({yunet_face_detector_0});
    // osd_0->attach_to({sface_face_encoder_0});

    yunet_face_detector_0->attach_to({file_src_0});

    file_src_0->start();

    // for debug purpose
    // tx_utils::AnalysisBoard board({file_src_0});
    // board.display(1, false);

    std::cout << "finish!!!" << std::endl;
    return 0;
}
