#pragma once

#include <memory>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include "nodes/node.h"
#include "nodes/des_node.h"
#include "objects/shapes/rect.h"
#include "nodes/stream_info_hookable.h"

namespace tx_utils {

// mainly used to store data from meta hookers' callback
struct MetaHookerStorage {
    int queue_size = -1;                                    // size of in/out queue of node
    int latency = 0;                                        // latency(ms) relative to src node at current port
    int called_count_since_epoch_start = -1;                // used for calculating fps at current port
    std::chrono::system_clock::time_point time_epoch_start; // used for calculating fps at current port
    std::shared_ptr<tx_objects::Meta> meta = nullptr;       // the latest meta (ptr) flowing through current port inside node (total 4 ports)
    std::string pre_fps;                                    // cache
};

// a class corresponding to vp_node, used to display node on screen and map the whole pipe from memery to screen.
class NodeOnScreen {
private:
    // orignal node in memery
    std::shared_ptr<tx_nodes::Node> original_node_ = nullptr;
    // node location and size on screen
    tx_objects::Rect node_rect_;
    // layer index in pipe
    int layer_;
    // nodes in next layer on screen
    std::vector<std::shared_ptr<NodeOnScreen>> next_nodes_on_screen_;

    // period to calculate fps, milliseconds
    int fps_epoch_ = 500;

    // reset fps if it has been long time no update, seconds
    int fps_timeout_ = 5;

    // font for display
    int font_face_ = cv::FONT_HERSHEY_SIMPLEX;

    // container to store data from meta hookers' callbacks
    MetaHookerStorage meta_arriving_hooker_storage;
    MetaHookerStorage meta_handling_hooker_storage;
    MetaHookerStorage meta_handled_hooker_storage;
    MetaHookerStorage meta_leaving_hooker_storage;

    // container to store data from stream info hooker's callback
    tx_nodes::StreamInfo stream_info_hooker_storage_;

    // container to store data from stream status hooker's callback
    tx_nodes::StreamStatus stream_status_hooker_storage_;

    // render configure
    const int node_title_h_ = 24;
    const int node_queue_width_ = 30;

    const int node_queue_port_w_h_ = 6;
    const int node_queue_port_padding_ = 8;
    const int node_gap_horizontal_ = 40;
    const int node_gap_vertical_ = 10;

public:
    NodeOnScreen(std::shared_ptr<tx_nodes::Node> original_node, tx_objects::Rect node_rect, int layer);
    ~NodeOnScreen();

    // render static parts for node, which keep unchanged all the time.
    void render_static_parts(cv::Mat &canvas);
    // render dynamic parts for node, which change frequently.
    void render_dynamic_parts(cv::Mat &canvas);

    std::shared_ptr<tx_nodes::Node> &get_orginal_node();

    std::vector<std::shared_ptr<NodeOnScreen>> &get_next_nodes_on_screen();
};
} // namespace tx_utils
