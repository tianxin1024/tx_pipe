#include <iostream>
#include "utils/logger.h"

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
    // auto file_src_0 = std::make_shared<tx_nodes::tx_file_src_node>(
    //     "file_src_0", 0, "../../workspace/face.mp4", 0.6);

    std::cout << "finish!!!" << std::endl;
    return 0;
}
