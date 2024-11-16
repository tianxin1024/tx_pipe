#pragma once

#include <opencv2/freetype.hpp>

#include "nodes/node.h"

namespace tx_nodes {

// on screen display(short as osd) node.
// another version for vp_frame_target display, display vp_sub_target at the bottom of screen.
class OsdNodeV2 : public Node {
private:
    // support chinese font
    cv::Ptr<cv::freetype::FreeType2> ft2;

    // leave a gap at the bottom of osd frame
    int gap_height = 256;
    int padding = 10;

protected:
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) override;

public:
    OsdNodeV2(std::string node_name, std::string font = "");
    ~OsdNodeV2();
};

} // namespace tx_nodes
