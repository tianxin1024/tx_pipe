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

    std::cout << "finish!!!" << std::endl;
    return 0;
}
