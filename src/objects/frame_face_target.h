
#pragma once
#include <vector>
#include <memory>
#include "objects/shapes/rect.h"

namespace tx_objects {

// target in frame detected by face detectors such as yunet.
// note: we can define new target type like vp_frame_xxx_target... if need (see vp_frame_pose_target also)
class FrameFaceTarget {
private:
    /* data */
public:
    FrameFaceTarget(int x,
                    int y,
                    int width,
                    int height,
                    float score,
                    std::vector<std::pair<int, int>> key_points = std::vector<std::pair<int, int>>(),
                    std::vector<float> embeddings = std::vector<float>());
    ~FrameFaceTarget();

    // x of top left
    int x;
    // y of top left
    int y;
    // width of rect
    int width;
    // height of rect
    int height;

    // confidence
    float score;

    // feature vector created by infer nodes such as vp_sface_feature_encoder_node.
    // embeddings can be used for face recognize or other reid works.
    std::vector<float> embeddings;

    // key points (5 points or more)
    std::vector<std::pair<int, int>> key_points;

    // track id filled by vp_track_node (child class) if it exists.
    int track_id = -1;
    // cache of track rects in the previous frames, filled by track node if it exists.
    // we can draw / analyse depend on these track rects later.
    std::vector<tx_objects::Rect> tracks;

    // clone myself
    std::shared_ptr<FrameFaceTarget> clone();

    // rect area of target
    Rect get_rect() const;
};

} // namespace tx_objects
