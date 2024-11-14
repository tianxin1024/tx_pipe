#include "objects/shapes/line.h"

namespace tx_objects {

Line::Line(Point start, Point end) :
    start_(start), end_(end) {
}

Line::~Line() {
}

float Line::length() {
    return start_.distance_with(end_);
}

} // namespace tx_objects
