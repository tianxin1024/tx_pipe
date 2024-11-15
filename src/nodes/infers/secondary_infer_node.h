#pragma once

#include "nodes/infer_node.h"

namespace tx_nodes {
// secondary infer node, it is the base class of infer node which MUST infer on small cropped image.
// note: detector such as yolo can be also applied on small cropped images.
class SecondaryInferNode : public InferNode {
private:
protected:
    // define how to prepare data
    virtual void prepare(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch, std::vector<cv::Mat> &mats_to_infer) override;
    bool need_apply(int primary_class_id, int target_width, int target_height);

public:
    SecondaryInferNode(std::string node_name,
                       std::string model_path,
                       std::string model_config_path = "",
                       std::string labels_path = "",
                       int input_width = 640,
                       int input_height = 640,
                       int batch_size = 1,
                       std::vector<int> p_class_ids_applied_to = std::vector<int>(),
                       int min_width_applied_to = 0,
                       int min_height_applied_to = 0,
                       int crop_padding = 10,
                       float scale = 1.0,
                       cv::Scalar mean = cv::Scalar(123.675, 116.28, 103.53), // imagenet dataset
                       cv::Scalar std = cv::Scalar(1),
                       bool swap_rb = true,
                       bool swap_chn = false);
    ~SecondaryInferNode();

    // we do secondary infer logic only if the primary class-id of target is in this vector.
    // not all targets in frame should be handled by vp_secondary_infer_node.
    std::vector<int> p_class_ids_applied_to;
    int crop_padding;
    // min height of target to be handled by vp_secondary_infer_node, 0 means no restriction
    int min_height_applied_to = 0;
    // min width of target to be handled by vp_secondary_infer_node，0 means no restriction
    int min_width_applied_to = 0;
};

} // namespace tx_nodes
