#pragma once

#include "nodes/primary_infer_node.h"
#include "objects/frame_face_target.h"

namespace tx_nodes {
// face detector based on YunNet
// https://github.com/opencv/opencv/blob/4.x/modules/objdetect/src/face_detect.cpp
// https://github.com/ShiqiYu/libfacedetection
class YunetFaceDetectorNode : public PrimaryInferNode {
private:
    // names of output layers in yunet
    const std::vector<std::string> out_names = {"loc", "conf", "iou"};
    float scoreThreshold = 0.7;
    float nmsThreshold = 0.5;
    int topK = 50;
    int inputW;
    int inputH;
    std::vector<cv::Rect2f> priors;
    void generatePriors();

protected:
    // override infer and preprocess as yunet has a different logic
    virtual void infer(const cv::Mat &blob_to_infer, std::vector<cv::Mat> &raw_outputs) override;
    virtual void preprocess(const std::vector<cv::Mat> &mats_to_infer, cv::Mat &blob_to_infer) override;

    virtual void postprocess(const std::vector<cv::Mat> &raw_outputs, const std::vector<std::shared_ptr<tx_objects::FrameMeta>> &frame_meta_with_batch) override;

public:
    YunetFaceDetectorNode(std::string node_name, std::string model_path, float score_threshold = 0.7, float nms_threshold = 0.5, int top_k = 50);
    ~YunetFaceDetectorNode();
};

} // namespace tx_nodes
