#pragma once

#define WITH_TRT true

#ifdef WITH_TRT
#include "nodes/primary_infer_node.h"
#include "../third_party/trt_yolov8/trt_yolov8_detector.h"

namespace tx_nodes {

// universal yolov8 detector based on tensorrt using third_party/trt_yolov8 library
class TrtYolov8Detector : public PrimaryInferNode {
private:
    std::shared_ptr<trt_yolov8::trt_yolov8_detector> yolov8_detector = nullptr;

protected:
    // we need a totally new logic for the whole infer combinations
    // no separate step pre-defined needed in base class
    virtual void run_infer_combinations(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) override;
    // override pure virtual method, for compile pass
    virtual void postprocess(const std::vector<cv::Mat> &raw_outputs, const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) override;

public:
    TrtYolov8Detector(std::string node_name, std::string model_path, std::string labels_path = "");
    ~TrtYolov8Detector();
};
} // namespace tx_nodes
#endif
