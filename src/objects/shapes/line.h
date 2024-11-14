#pragma once

#include "objects/shapes/point.h"

namespace tx_objects {

// line in 2-dims coordinate system
class Line {
private:
    /* data */
public:
    Line() = default;
    Line(Point start, Point end);
    ~Line();

    Point start_;
    Point end_;

    // distance between start and end point
    float length();
};

} // namespace tx_objects
