#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include "nodes/des_node.h"

namespace tx_nodes {

// screen des node, display video on local window.
class ScreenDesNode : public DesNode {
private:
    /* data */
    std::string gst_template = "appsrc ! videoconvert ! videoscale ! textoverlay text=%s halignment=left valignment=top font-desc='Sans,16' shaded-background=true ! timeoverlay halignment=right valignment=top font-desc='Sans,16' shaded-background=true ! queue ! fpsdisplaysink video-sink=ximagesink sync=false";
    cv::VideoWriter screen_writer;

protected:
    // re-implementation, return nullptr.
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) override;
    // re-implementation, return nullptr.
    virtual std::shared_ptr<tx_objects::Meta> handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) override;

public:
    ScreenDesNode(std::string node_name,
                  int channel_index,
                  bool osd = true,
                  tx_objects::Size display_w_h = {});
    ~ScreenDesNode();

    // for osd frame
    bool osd;
    // display size
    tx_objects::Size display_w_h;
};
} // namespace tx_nodes
