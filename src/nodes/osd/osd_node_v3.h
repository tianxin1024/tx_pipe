#pragma once

#include <opencv2/freetype.hpp>

#include "nodes/node.h"

namespace tx_nodes {

// on screen display(short as osd) node.
// another version for vp_frame_target display, display mask area for image segmentation.
class OsdNodeV3 : public Node {
private:
    // support chinese font
    cv::Ptr<cv::freetype::FreeType2> ft2;
    float mask_threshold = 0.3;

protected:
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) override;

public:
    OsdNodeV3(std::string node_name, std::string font = "");
    ~OsdNodeV3();
};
} // namespace tx_nodes
