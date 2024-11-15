#pragma once

#include <string>
#include <memory>
#include <vector>

#include "objects/shapes/rect.h"

namespace tx_objects {

// sub target inside vp_frame_target, created by detectors which MUST infer on small cropped images (detectors are derived from vp_secondary_infer_node).
// this class has less properties/functions than vp_frame_target.
// see vp_frame_target also.
class SubTarget {
private:
    /* data */
public:
    SubTarget(int x,
              int y,
              int width,
              int height,
              int class_id,
              float score,
              std::string label,
              int frame_index,
              int channel_index);
    ~SubTarget();

    // x of top left
    int x;
    // y of top left
    int y;
    // width of rect
    int width;
    // height of rect
    int height;

    // class id
    int class_id;
    // score
    float score;
    // label
    std::string label;

    // frame the sub target belongs to
    int frame_index;
    // channel the sub target belongs to
    int channel_index;

    // save some info
    std::vector<std::string> attachments;

    // clone myself
    std::shared_ptr<SubTarget> clone();

    // rect area of target
    Rect get_rect() const;
};

} // namespace tx_objects
