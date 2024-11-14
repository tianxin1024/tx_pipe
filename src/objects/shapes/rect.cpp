

#include "objects/shapes/rect.h"

namespace tx_objects {

Rect::Rect(int x, int y, int width, int height) :
    x_(x),
    y_(y),
    width_(width),
    height_(height) {
}

Rect::Rect(Point left_top, Size wh) :
    x_(left_top.x), y_(left_top.y), width_(wh.width_), height_(wh.height_) {
}

Rect::~Rect() {
}

Point Rect::center() {
    return Point(x_ + width_ / 2, y_ + height_ / 2);
}

float Rect::iou_with(const Rect &rect) {
    return 1.0;
}

bool Rect::contains(const Point &p) {
    return true;
}

Point Rect::track_point() {
    // by default the center point of bottom is tracking point.
    return {x_ + width_ / 2, y_ + height_};
}
} // namespace tx_objects
