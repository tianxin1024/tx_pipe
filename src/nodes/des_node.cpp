#include "nodes/des_node.h"

namespace tx_nodes {

DesNode::DesNode(std::string node_name, int channel_index) :
    Node(node_name), channel_index_(channel_index) {
    stream_status_.channel_index = channel_index;
}

DesNode::~DesNode() {
}

// do nothing in des nodes
void DesNode::dispatch_run() {
    // dispatch thread terminates immediately in all des nodes
}

// it is the end point of stream(frame meta) in pipe, this method MUST be called at the end of handle_frame_meta in derived class.
// using 'return vp_des_node::handle_frame_meta(...);'
std::shared_ptr<tx_objects::Meta> DesNode::handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) {
    // update cache of stream status
    stream_status_.frame_index = meta->frame_index_;
    stream_status_.width = meta->frame_.size().width;
    stream_status_.height = meta->frame_.size().height;
    stream_status_.direction = to_string();

    // calculate the duration between now and the time when meta created, which is latency.
    auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - meta->create_time_);
    stream_status_.latency = delta_time.count();

    // update fps if need
    fps_counter_++;
    delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - fps_last_time_);
    if (delta_time.count() >= fps_epoch_) {
        stream_status_.fps = fps_counter_ * 1000.0 / delta_time.count();
        fps_counter_ = 0;
        fps_last_time_ = std::chrono::system_clock::now();
    }

    // activate the stream status hooker if need
    invoke_stream_status_hooker(node_name_, stream_status_);

    return nullptr;
}

// it is the end point of control meta in pipe, this method MUST be called at the end of handle_control_meta in derived class.
// using 'return vp_des_node::handle_control_meta(...);'
std::shared_ptr<tx_objects::Meta> DesNode::handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) {
    // ...
    return nullptr;
}

NodeType DesNode::node_type() {
    return NodeType::DES;
}
} // namespace tx_nodes
