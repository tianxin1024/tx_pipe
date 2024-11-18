#include "nodes/infers/trt_yolov8_detector.h"
#ifdef WITH_TRT

namespace tx_nodes {

TrtYolov8Detector::TrtYolov8Detector(std::string node_name,
                                     std::string model_path,
                                     std::string labels_path) :
    PrimaryInferNode(node_name, "", "", labels_path) {
    yolov8_detector = std::make_shared<trt_yolov8::trt_yolov8_detector>(model_path);
    this->initialized();
}

TrtYolov8Detector::~TrtYolov8Detector() {
    deinitialized();
}

// please refer to vp_infer_node::run_infer_combinations
void TrtYolov8Detector::run_infer_combinations(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) {
    assert(frame_meta_with_batch.size() == 1);
    std::vector<cv::Mat> mats_to_infer;

    // start
    auto start_time = std::chrono::system_clock::now();

    // prepare data, as same as base class
    PrimaryInferNode::prepare(frame_meta_with_batch, mats_to_infer);
    auto prepare_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    start_time = std::chrono::system_clock::now();
    std::vector<std::vector<Detection>> detections;
    yolov8_detector->detect(mats_to_infer, detections);

    assert(detections.size() == 1);
    auto &detection_list = detections[0];
    auto &frame_meta = frame_meta_with_batch[0];

    for (int i = 0; i < detection_list.size(); i++) {
        auto &objbox = detection_list[i];

        // objbox.bbox: center_x center_y width height
        auto rect = get_rect(frame_meta->frame_, objbox.bbox); // convert to: x, y, width,height

        // check value range
        int x = std::max(rect.x, 0);
        int y = std::max(rect.y, 0);
        int width = std::min(rect.width, frame_meta->frame_.cols - x);
        int height = std::min(rect.height, frame_meta->frame_.rows - y);
        if (width <= 0 || height <= 0) {
            continue;
        }
        auto label = labels.size() == 0 ? "" : labels[objbox.class_id];
        auto target = std::make_shared<tx_objects::FrameTarget>(x, y, width, height,
                                                                objbox.class_id, objbox.conf, frame_meta->frame_index_, frame_meta->channel_index_, label);
        // create target and update back into frame meta
        frame_meta->targets.push_back(target);
    }
    auto infer_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time);

    // can not calculate preprocess time and postprocess time, set 0 by default.
    InferNode::infer_combinations_time_cost(mats_to_infer.size(), prepare_time.count(), 0, infer_time.count(), 0);
}

void TrtYolov8Detector::postprocess(const std::vector<cv::Mat> &raw_outputs, const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) {
}
} // namespace tx_nodes
#endif
