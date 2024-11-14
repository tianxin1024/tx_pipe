
#pragma once

#include <utility>
#include <cmath>

namespace tx_objects {

// point in 2-dims coordinate system
class Point {
private:
    /* data */
public:
    Point(int x = 0, int y = 0);
    ~Point();

    int x;
    int y;

    // distance between 2 points
    float distance_with(const Point &p);
};
} // namespace tx_objects
