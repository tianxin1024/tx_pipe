#pragma once

#include "nodes/primary_infer_node.h"
// #include "yolo.h"

namespace tx_nodes {

// yolo detector based on rknn
class RkFirstYolo : public PrimaryInferNode {
private:
    // std::shared_ptr<YOLO> rk_model;

protected:
    // we need a totally new logic for the whole infer combinations
    // no separate step pre-defined needed in base class
    virtual void run_infer_combinations(const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) override;
    // override pure virtual method, for compile pass
    virtual void postprocess(const std::vector<cv::Mat> &raw_outputs,
                             const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) override;

public:
    RkFirstYolo(std::string node_name, std::string json_path);
    ~RkFirstYolo();
}; // end of class RkFirstYolo

} // namespace tx_nodes
