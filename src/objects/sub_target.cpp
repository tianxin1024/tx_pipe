#include "objects/sub_target.h"

namespace tx_objects {

SubTarget::SubTarget(int x,
                     int y,
                     int width,
                     int height,
                     int class_id,
                     float score,
                     std::string label,
                     int frame_index,
                     int channel_index) :
    x(x),
    y(y),
    width(width),
    height(height),
    class_id(class_id),
    score(score),
    label(label),
    frame_index(frame_index),
    channel_index(channel_index) {
}

SubTarget::~SubTarget() {
}

std::shared_ptr<SubTarget> SubTarget::clone() {
    return std::make_shared<SubTarget>(*this);
}

Rect SubTarget::get_rect() const {
    return Rect(x, y, width, height);
}

} // namespace tx_objects
