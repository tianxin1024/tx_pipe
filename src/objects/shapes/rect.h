#pragma once

#include <tuple>

#include "objects/shapes/point.h"
#include "objects/shapes/size.h"

namespace tx_objects {

// rect in 2-dims coordinate system
class Rect {
private:
    /* data */
public:
    Rect() = default;
    Rect(int x, int y, int width, int height);
    Rect(Point left_top, Size wh);
    ~Rect();

    int x_;
    int y_;
    int width_;
    int height_;

    // get center point of the rect
    Point center();

    // get track point of the rect
    // track point is used to locate the target(represented by the rect)
    Point track_point();

    // calculate the iou with another rect
    float iou_with(const Rect &rect);

    // check if the rect contains a point
    bool contains(const Point &p);
};

} // namespace tx_objects
