#include "objects/shapes/point.h"

namespace tx_objects {

Point::Point(int x, int y) :
    x(x), y(y) {
}

Point::~Point() {
}

float Point::distance_with(const Point &p) {
    return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2));
}

} // namespace tx_objects
