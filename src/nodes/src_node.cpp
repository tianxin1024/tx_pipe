#include <memory>
#include "nodes/src_node.h"
// #include "objects/image_record_control_meta.h"
// #include "objects/video_record_control_meta.h"

namespace tx_nodes {

SrcNode::SrcNode(std::string node_name,
                 int channel_index,
                 float resize_ratio) :
    Node(node_name),
    channel_index_(channel_index),
    resize_ratio_(resize_ratio),
    frame_index_(-1) {
    assert(resize_ratio > 0 && resize_ratio <= 1.0f);
}

SrcNode::~SrcNode() {
}

void SrcNode::deinitialized() {
    alive_ = false;
    gate_.open();
    Node::deinitialized();
}

void SrcNode::handle_run() {
    throw tx_excepts::NotImplementedError("must have re-implementaion for 'handle_run' method in src nodes!");
}

std::shared_ptr<tx_objects::Meta>
SrcNode::handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) {
    throw tx_excepts::InvalidCallingError("'handle_frame_meta' method could not be called in src nodes!");
}

std::shared_ptr<tx_objects::Meta>
SrcNode::handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) {
    throw tx_excepts::InvalidCallingError("'handle_control_meta' method could not be called in src nodes!");
}

void SrcNode::start() {
    gate_.open();
}

void SrcNode::stop() {
    gate_.close();
}

void SrcNode::speak() {
    auto speak_control_meta = std::make_shared<tx_objects::ControlMeta>(tx_objects::ControlType::SPEAK, this->channel_index_);
    this->push_meta(speak_control_meta);
}

NodeType SrcNode::node_type() {
    return NodeType::SRC;
}

int SrcNode::get_original_fps() const {
    return original_fps_;
}

int SrcNode::get_original_width() const {
    return original_width_;
}

int SrcNode::get_original_height() const {
    return original_height_;
}

void SrcNode::record_video_manually(bool osd, int video_duration) {
    // make sure file is not too long
    assert(video_duration <= 60 && video_duration >= 5);

    // generate file_name_without_ext
    // MUST be unique
    // auto file_name_without_ext = tx_utils::time_format(NOW, "manually_record_video_<year><mon><day><hour><min><sec><mili>");

    // // create control meta
    // auto video_record_control_meta = std::make_shared<tx_objects::video_record_control_meta>(channel_index, file_name_without_ext, video_duration, osd);

    // // push meta to pipe
    // push_meta(video_record_control_meta);
}

void SrcNode::record_image_manually(bool osd) {
    // generate file_name_without_ext
    // MUST be unique
    // auto file_name_without_ext = tx_utils::time_format(NOW, "manually_record_image_<year><mon><day><hour><min><sec><mili>");

    // // create control meta
    // auto image_record_control_meta = std::make_shared<tx_objects::vp_image_record_control_meta>(channel_index, file_name_without_ext, osd);

    // // push meta to pipe
    // push_meta(image_record_control_meta);
}
} // namespace tx_nodes
