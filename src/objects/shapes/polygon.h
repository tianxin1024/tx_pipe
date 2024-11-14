#pragma once

#include <vector>
#include "objects/shapes/point.h"

namespace tx_objects {

class Polygon {
private:
    /* data */
public:
    Polygon() = default;
    Polygon(std::vector<Point> vertexs);
    ~Polygon();

    // vertexs of the polygon
    std::vector<Point> vertexs_;

    // check if the polygon contains a point
    bool contains(const Point &p);
};

} // namespace tx_objects
