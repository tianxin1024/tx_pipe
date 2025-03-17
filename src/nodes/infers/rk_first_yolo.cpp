#include "nodes/infers/rk_first_yolo.h"

namespace tx_nodes {

RkFirstYolo::RkFirstYolo(std::string node_name, std::string json_path) :
    PrimaryInferNode(node_name, "") {
    // YoloConfig conf;
    // int ret = YOLO::load_config(json_path, conf);
    // this->initialized();
}

RkFirstYolo::~RkFirstYolo() {
    // deinitialized();
}
void RkFirstYolo::run_infer_combinations(
    const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) {
}

void RkFirstYolo::postprocess(const std::vector<cv::Mat> &raw_outputs,
                              const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) {
}

} // namespace tx_nodes
