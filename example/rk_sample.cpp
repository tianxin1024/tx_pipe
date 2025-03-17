#include <iostream>
#include "utils/logger.h"
#include "nodes/file_src_node.h"
#include "utils/analysis_board.h"
#include "nodes/infers/rk_first_yolo.h"
#include "nodes/infers/sface_feature_encoder_node.h"
#include "nodes/osd/face_osd_node_v2.h"
#include "nodes/screen_des_node.h"
#include "nodes/rk_rtsp_src_node.h"

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
    auto src_0 = std::make_shared<tx_nodes::RkRtspSrcNode>(
        "rtsp_src_0", 0, "rtsp://localhost:8554/stream");

    auto yolo_0 = std::make_shared<tx_nodes::RkFirstYolo>(
        "rk_yolo_0", "assert/configs/person.json");

    // yolo_0->attach_to({src_0});

    return 0;
}
