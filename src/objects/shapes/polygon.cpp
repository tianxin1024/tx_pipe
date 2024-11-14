#include <assert.h>
#include "objects/shapes/polygon.h"

namespace tx_objects {

Polygon::Polygon(std::vector<Point> vertexs) :
    vertexs_(vertexs) {
    assert(vertexs.size() > 2);
}

Polygon::~Polygon() {
}

bool Polygon::contains(const Point &p) {
    return true;
}

} // namespace tx_objects
