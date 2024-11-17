
#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "nodes/node.h"

namespace tx_nodes {

// on screen display(short as osd) node.
// another version for vp_frame_face_target display, including displaying similarity between faces.
class FaceOsdNodeV2 : public Node {
private:
    // leave a gap at the bottom of osd frame
    int gap_height = 112;
    int padding = 5;

    // cache for display
    cv::Mat the_baseline_face;                     // the first face detected in frames
    std::vector<float> the_baseline_face_feature;  // the feature of first face in frames
    std::vector<cv::Mat> faces_list;               // faces detected except the first one
    std::vector<std::vector<float>> face_features; // face features except the first one
    std::vector<float> cosine_distances;           // cosine distance between face and the first one
    std::vector<float> l2_distances;               // l2 distance between face and the first one

    double cosine_similar_thresh = 0.363; // higher value means higher similarity, max 1.0
    double l2norm_similar_thresh = 1.128; // lower value means higher similarity, min 0.0

    // calculate distance
    // 0 cosine distance
    // 1 l2 distance
    double match(std::vector<float> &feature1, std::vector<float> &feature2, int dis_type);

protected:
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) override;

public:
    FaceOsdNodeV2(std::string node_name);
    ~FaceOsdNodeV2();
};
} // namespace tx_nodes
