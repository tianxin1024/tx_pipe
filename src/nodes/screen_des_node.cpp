#include "nodes/screen_des_node.h"
#include "utils/utils.h"

namespace tx_nodes {

ScreenDesNode::ScreenDesNode(std::string node_name,
                             int channel_index,
                             bool osd,
                             tx_objects::Size display_w_h) :
    DesNode(node_name, channel_index),
    osd(osd),
    display_w_h(display_w_h) {
    this->gst_template = tx_utils::string_format(this->gst_template, node_name.c_str());
    TX_INFO(tx_utils::string_format("[%s] [%s]", node_name.c_str(), gst_template.c_str()));
    this->initialized();
}

ScreenDesNode::~ScreenDesNode() {
    deinitialized();
}

// re-implementation, return nullptr.
std::shared_ptr<tx_objects::Meta>
ScreenDesNode::handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) {
    TX_DEBUG(tx_utils::string_format("[%s] received frame meta, channel_index=>%d, frame_index=>%d", node_name_.c_str(), meta->channel_index_, meta->frame_index_));

    cv::Mat resize_frame;
    if (this->display_w_h.width_ != 0 && this->display_w_h.height_ != 0) {
        cv::resize((osd && !meta->osd_frame_.empty()) ? meta->osd_frame_ : meta->frame_, resize_frame, cv::Size(display_w_h.width_, display_w_h.height_));
    } else {
        resize_frame = (osd && !meta->osd_frame_.empty()) ? meta->osd_frame_ : meta->frame_;
    }

    if (!screen_writer.isOpened()) {
        assert(screen_writer.open(this->gst_template, cv::CAP_GSTREAMER, 0, meta->fps_, {resize_frame.cols, resize_frame.rows}));
    }
    screen_writer.write(resize_frame);

    // for general works defined in base class
    return DesNode::handle_frame_meta(meta);
}

// re-implementation, return nullptr.
std::shared_ptr<tx_objects::Meta>
ScreenDesNode::handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) {
    // for general works defined in base class
    return DesNode::handle_control_meta(meta);
}
} // namespace tx_nodes
