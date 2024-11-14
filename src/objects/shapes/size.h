#pragma once

#include <utility>

namespace tx_objects {

// size(width and height) in 2-dims coordinate system
class Size {
private:
    /* data */
public:
    Size(int width = 0, int height = 0);
    ~Size();

    int width_;
    int height_;
};

} // namespace tx_objects
