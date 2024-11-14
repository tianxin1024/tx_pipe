#include <iterator>

#include "objects/frame_meta.h"

namespace tx_objects {

FrameMeta::FrameMeta(cv::Mat frame, int frame_index, int channel_index, int original_width, int original_height, int fps) :
    Meta(MetaType::FRAME, channel_index),
    frame_index_(frame_index),
    original_width_(original_width),
    original_height_(original_height),
    fps_(fps),
    frame_(frame) {
    assert(!frame.empty());
}

// copy constructor of vp_frame_meta would NOT be called at most time.
// only when it flow through vp_split_node with vp_split_node::split_with_deep_copy==true.
// in fact, all kinds of meta would NOT be copyed in its lifecycle, we just pass them by poniter most time.
FrameMeta::FrameMeta(const FrameMeta &meta) :
    Meta(meta),
    frame_index_(meta.frame_index_),
    original_width_(meta.original_width_),
    original_height_(meta.original_height_),
    fps_(meta.fps_) {
    // deep copy frame data
    this->frame_ = meta.frame_.clone();
    this->osd_frame_ = meta.osd_frame_.clone();
    this->mask_ = meta.mask_.clone();

    // deep copy targets
    // for (auto &i : meta.targets) {
    //     this->targets_.push_back(i->clone());
    // }
    // // deep copy pose targets
    // for (auto &i : meta.pose_targets_) {
    //     this->pose_targets.push_back(i->clone());
    // }
    // // deep copy face targets
    // for (auto &i : meta.face_targets) {
    //     this->face_targets.push_back(i->clone());
    // }
    // // deep copy text targets
    // for (auto &i : meta.text_targets) {
    //     this->text_targets.push_back(i->clone());
    // }
    // // deep copy ba results
    // for (auto &i : meta.ba_results) {
    //     this->ba_results.push_back(i->clone());
    // }
}

FrameMeta::~FrameMeta() {
}

std::shared_ptr<Meta> FrameMeta::clone() {
    // just call copy constructor and return new pointer
    return std::make_shared<FrameMeta>(*this);
}

// std::vector<std::shared_ptr<FrameTarget>> vp_frame_meta::get_targets_by_ids(const std::vector<int> &ids) {
//     std::vector<std::shared_ptr<vp_objects::vp_frame_target>> results;
//     for (auto &t : targets) {
//         if (std::find(ids.begin(), ids.end(), t->track_id) != ids.end()) {
//             results.push_back(t);
//         }
//     }
//     return results;
// }
} // namespace tx_objects
