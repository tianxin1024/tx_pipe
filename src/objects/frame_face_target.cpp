#include "objects/frame_face_target.h"

namespace tx_objects {

FrameFaceTarget::FrameFaceTarget(int x,
                                 int y,
                                 int width,
                                 int height,
                                 float score,
                                 std::vector<std::pair<int, int>> key_points,
                                 std::vector<float> embeddings) :
    x(x),
    y(y),
    width(width),
    height(height),
    score(score),
    key_points(key_points),
    embeddings(embeddings) {
}

FrameFaceTarget::~FrameFaceTarget() {
}

std::shared_ptr<FrameFaceTarget> FrameFaceTarget::clone() {
    return std::make_shared<FrameFaceTarget>(*this);
}

Rect FrameFaceTarget::get_rect() const {
    return Rect(x, y, width, height);
}
} // namespace tx_objects
