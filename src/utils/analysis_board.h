#pragma once

#include <vector>
#include <map>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

#include "nodes/node.h"
#include "objects/shapes/rect.h"

#include "utils/node_on_screen.h"

namespace tx_utils {

class AnalysisBoard final {
private:
    // configure for render
    const int node_width_ = 140;
    const int node_height_ = 140;
    const int canvas_gap_horizontal_ = 120;
    const int canvas_gap_vertical_ = 60;
    const int node_gap_horizontal_ = 40;
    const int node_gap_vertical_ = 10;

    std::string board_title_ = "analysis_board";
    bool alive_ = true;

    int canvas_width_ = 0;
    int canvas_height_ = 0;

    std::string gst_template_ = "appsrc ! videoconvert ! x264enc bitrate=%d ! h264parse ! flvmux ! rtmpsink location=%s";
    cv::VideoWriter rtmp_writer_;

    //
    bool displaying_ = false;

    // width of pipe
    int pipe_width_;
    // height of pipe
    int pipe_height_;

    // start points of pipe
    std::vector<std::shared_ptr<tx_nodes::Node>> src_nodes_in_pipe_;

    // cache for easy access purpose
    std::vector<std::shared_ptr<NodeOnScreen>> src_nodes_on_screen;
    std::vector<std::shared_ptr<NodeOnScreen>> des_nodes_on_screen;

    // canvas to draw
    cv::Mat bg_canvas_;

    // display thread(on screen)
    std::thread display_th_;

    // display thread(via rtmp)
    std::thread rtmp_th_;

    // render nodes in a layer
    void render_layer(std::vector<std::shared_ptr<NodeOnScreen>> nodes_in_layer, cv::Mat &canvas, bool static_parts = true);

    // map nodes in memory to screen, one layer by layer.
    void map_nodes(std::vector<std::shared_ptr<NodeOnScreen>> nodes_on_screen, int layer);

    // initialize resource
    void init();

    // sync for reload operation
    std::mutex reload_lock_;

    // tool methods
    std::function<int(int)> layer_base_left_cal = [=](int layer_index) { return canvas_gap_horizontal_ + layer_index * (node_width_ + node_gap_horizontal_); };
    std::function<int(int)> layer_base_top_cal = [=](int num_nodes_in_layer) { return (canvas_height_ - (num_nodes_in_layer * node_height_ + (num_nodes_in_layer - 1) * node_gap_vertical_)) / 2; };

public:
    AnalysisBoard(std::vector<std::shared_ptr<tx_nodes::Node>> src_nodes_in_pipe);
    ~AnalysisBoard();

    // save pipe structure to png
    void save(std::string path);

    // display pipe on screen and refresh it automatically
    void display(int interval = 1, bool block = true);

    // display pipe by rtmp and refresh it automatically
    void push_rtmp(std::string rtmp, int bitrate = 1024);

    // reload pipeline with new src nodes
    void reload(std::vector<std::shared_ptr<tx_nodes::Node>> new_src_nodes_in_pipe = std::vector<std::shared_ptr<tx_nodes::Node>>());
};
} // namespace tx_utils
