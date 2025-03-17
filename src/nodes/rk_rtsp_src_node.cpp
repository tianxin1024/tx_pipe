#include "nodes/rk_rtsp_src_node.h"

namespace tx_nodes {

RkRtspSrcNode::RkRtspSrcNode(std::string node_name,
                             int channel_index,
                             std::string rtsp_url,
                             float resize_ratio,
                             int skip_interval) :
    SrcNode(node_name, channel_index, resize_ratio),
    rtsp_url(rtsp_url), skip_interval(skip_interval) {
    // TODO tianx ...
}

RkRtspSrcNode::~RkRtspSrcNode() {
    deinitialized();
}

void RkRtspSrcNode::handle_run() {
}

std::string RkRtspSrcNode::to_string() {
}

} // namespace tx_nodes
