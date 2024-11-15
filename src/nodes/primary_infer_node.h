#pragma once

#include "nodes/infer_node.h"

namespace tx_nodes {

// primary infer node, it is the base class of infer node which MUST infer on the whole frame.
class PrimaryInferNode : public InferNode {
private:
protected:
    // define how to prepare data
    virtual void prepare(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch, std::vector<cv::Mat> &mats_to_infer) override;

public:
    PrimaryInferNode(std::string node_name,
                     std::string model_path,
                     std::string model_config_path = "",
                     std::string labels_path = "",
                     int input_width = 640,
                     int input_height = 640,
                     int batch_size = 1,
                     int class_id_offset = 0,
                     float scale = 1.0,
                     cv::Scalar mean = cv::Scalar(123.675, 116.28, 103.53), // imagenet dataset
                     cv::Scalar std = cv::Scalar(1),
                     bool swap_rb = true,
                     bool swap_chn = false);
    ~PrimaryInferNode();

    // because maybe multi primary infer nodes exist in one pipeline,
    // we need an offset value to ensure each class id is unique and has not conflict.
    // the class_id_offset should be the sum of class ids in previous vp_primary_infer_node of pipe.
    int class_id_offset;
};
} // namespace tx_nodes
