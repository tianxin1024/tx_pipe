#include "objects/frame_target.h"

namespace tx_objects {

FrameTarget::FrameTarget(int x,
                         int y,
                         int width,
                         int height,
                         int primary_class_id,
                         float primary_score,
                         int frame_index,
                         int channel_index,
                         std::string primary_label) :
    x(x),
    y(y),
    width(width),
    height(height),
    primary_class_id(primary_class_id),
    primary_score(primary_score),
    primary_label(primary_label),
    frame_index(frame_index),
    channel_index(channel_index),
    ba_flags(0) {
}

FrameTarget::FrameTarget(Rect rect,
                         int primary_class_id,
                         float primary_score,
                         int frame_index,
                         int channel_index,
                         std::string primary_label) :
    FrameTarget(rect.x_,
                rect.y_,
                rect.width_,
                rect.height_,
                primary_class_id,
                primary_score,
                frame_index,
                channel_index,
                primary_label) {
}

FrameTarget::~FrameTarget() {
}

std::shared_ptr<FrameTarget> FrameTarget::clone() {
    return std::make_shared<FrameTarget>(*this);
}

Rect FrameTarget::get_rect() const {
    return Rect(x, y, width, height);
}
} // namespace tx_objects
