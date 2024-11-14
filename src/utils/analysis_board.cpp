#include "utils/analysis_board.h"
#include "utils/pipe_checker.h"

namespace tx_utils {

AnalysisBoard::AnalysisBoard(std::vector<std::shared_ptr<tx_nodes::Node>> src_nodes_in_pipe) :
    src_nodes_in_pipe_(src_nodes_in_pipe) {
    init();
}

AnalysisBoard::~AnalysisBoard() {
    // set alive to false and wait threads exits
    alive_ = false;
    if (display_th_.joinable()) {
        display_th_.join();
    }
    if (rtmp_th_.joinable()) {
        rtmp_th_.join();
    }
}

void AnalysisBoard::init() {
    src_nodes_on_screen.clear();
    des_nodes_on_screen.clear();

    // check pipe
    PipeChecker pipe_checker;
    pipe_checker(src_nodes_in_pipe_);

    // layers number and max nodes number of all layers
    pipe_width_ = pipe_checker.pipe_width();
    pipe_height_ = pipe_checker.pipe_height();

    // calculate the w and h of canvas
    canvas_width_ = pipe_width_ * node_width_ + (pipe_width_ - 1) * node_gap_horizontal_ + 2 * canvas_gap_horizontal_;
    canvas_height_ = pipe_height_ * node_height_ + (pipe_height_ - 1) * node_gap_vertical_ + 2 * canvas_gap_vertical_;

    // create canvas Mat and initialize it with white
    bg_canvas_.create(canvas_height_, canvas_width_, CV_8UC3);
    bg_canvas_ = cv::Scalar(255, 255, 255);

    // map recursively
    map_nodes(src_nodes_on_screen, 1);

    // render static parts starting with 1st layer
    render_layer(src_nodes_on_screen, bg_canvas_);

    // save to local by default
    save(board_title_ + ".png");
}
void AnalysisBoard::reload(std::vector<std::shared_ptr<tx_nodes::Node>> new_src_nodes_in_pipe) {
    std::lock_guard<std::mutex> guard(reload_lock_);
    if (new_src_nodes_in_pipe.size() != 0) {
        this->src_nodes_in_pipe_ = new_src_nodes_in_pipe;
    }
    init();
}

void AnalysisBoard::save(std::string path) {
    cv::imwrite(path, bg_canvas_);
}

void AnalysisBoard::display(int interval, bool block) {
    assert(interval > 0);
    if (displaying_) {
        return;
    }

    auto display_func = [&, interval]() {
            while (alive_) {
                auto loop_start = std::chrono::system_clock::now();
                {
                    std::lock_guard<std::mutex> guard(reload_lock_);  // in case it reloading
                    // deep copy the static background
                    cv::Mat mat_to_display = bg_canvas_.clone();
                    // render dynamic parts starting with 1 st layer
                    render_layer(src_nodes_on_screen, mat_to_display, false);
                    cv::imshow(board_title_, mat_to_display);
                }

                // calculate the time need wait for
                auto loop_cost = std::chrono::system_clock::now() - loop_start;
                auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(interval) - loop_cost);

                cv::waitKey(wait_time.count());
            } };
    displaying_ = true;
    display_th_ = std::thread(display_func);
    if (block) {
        display_th_.join();
    }
}

void AnalysisBoard::push_rtmp(std::string rtmp, int bitrate) {
    if (displaying_) {
        return;
    }
    auto fps = 10;
    auto rtmp_url = tx_utils::string_format(gst_template_, bitrate, rtmp.c_str());
    // 10 fps by default
    assert(rtmp_writer_.open(rtmp_url, cv::CAP_GSTREAMER, fps, {bg_canvas_.cols, bg_canvas_.rows}));

    auto display_func = [&, fps]() {
            while (alive_) {
                auto loop_start = std::chrono::system_clock::now();
                {
                    std::lock_guard<std::mutex> guard(reload_lock_); // in case it reloading
                    // deep copy the static background
                    cv::Mat mat_to_display = bg_canvas_.clone();
                    // render dynamic parts starting with 1 st layer
                    render_layer(src_nodes_on_screen, mat_to_display, false);
                    rtmp_writer_.write(mat_to_display);
                }

                // calculate the time need wait for
                auto loop_cost = std::chrono::system_clock::now() - loop_start;
                auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds(1000 / fps) - loop_cost);

                std::this_thread::sleep_for(wait_time);
            } };
    displaying_ = true;
    rtmp_th_ = std::thread(display_func);
}

void AnalysisBoard::render_layer(std::vector<std::shared_ptr<NodeOnScreen>> nodes_in_layer, cv::Mat &canvas, bool static_parts) {
    std::vector<std::shared_ptr<NodeOnScreen>> nodes_in_next_layer;
    for (auto &i : nodes_in_layer) {
        if (static_parts) {
            i->render_static_parts(canvas);
        } else {
            i->render_dynamic_parts(canvas);
        }
        auto n = i->get_next_nodes_on_screen();
        nodes_in_next_layer.insert(nodes_in_next_layer.end(), n.begin(), n.end());
    }

    if (nodes_in_next_layer.size() > 0) {
        bool all_the_same = true;
        for (auto &i : nodes_in_next_layer) {
            if (i != nodes_in_next_layer[0]) {
                all_the_same = false;
                break;
            }
        }
        // just keep the first one if all the next nodes are the same
        if (all_the_same) {
            nodes_in_next_layer.erase(nodes_in_next_layer.begin() + 1, nodes_in_next_layer.end());
        }

        render_layer(nodes_in_next_layer, canvas, static_parts);
    } else { // recursion end

        /* global drawing */

        // draw layer index at the bottom of canvas
        if (static_parts) {
            for (int i = 0; i < pipe_width_; i++) {
                /* code */
                cv::putText(canvas, "layer_" + std::to_string(i + 1), cv::Point(canvas_gap_horizontal_ + (node_width_ + node_gap_horizontal_) * i, canvas_height_ - int(canvas_gap_vertical_ / 3)), 1, 1, cv::Scalar(255, 0, 0));
            }
        }

        // refresh time at the top left of canvas
        if (!static_parts) {
            auto time = tx_utils::time_format(NOW, "<hour>:<min>:<sec>");
            cv::putText(canvas, time, cv::Point(20, 20), 1, 1, cv::Scalar(255, 0, 0));
        }
    }
}

void AnalysisBoard::map_nodes(std::vector<std::shared_ptr<NodeOnScreen>> nodes_in_layer, int layer) {
    if (layer == 1) {
        // here nodes_in_layer is empty
        auto num_src_nodes_in_pipe = src_nodes_in_pipe_.size();

        auto base_left = layer_base_left_cal(layer - 1);
        auto base_top = layer_base_top_cal(num_src_nodes_in_pipe);

        // map nodes at 1st layer in memory to screen
        for (int i = 0; i < num_src_nodes_in_pipe; i++) {
            auto node_left = base_left;
            auto node_top = base_top + i * (node_height_ + node_gap_vertical_);

            auto node_on_screen = std::make_shared<NodeOnScreen>(src_nodes_in_pipe_[i], tx_objects::Rect(node_left, node_top, node_width_, node_height_), layer);
            src_nodes_on_screen.push_back(node_on_screen);
        }

        map_nodes(src_nodes_on_screen, layer + 1);
    } else {
        std::vector<std::shared_ptr<tx_nodes::Node>> all_nodes_in_next_layer;
        for (auto &i : nodes_in_layer) {
            auto next_nodes = i->get_orginal_node()->next_nodes();
            all_nodes_in_next_layer.insert(all_nodes_in_next_layer.end(), next_nodes.begin(), next_nodes.end());
        }
        if (all_nodes_in_next_layer.size() > 0) {
            bool all_the_same = true;
            for (auto &i : all_nodes_in_next_layer) {
                if (i != all_nodes_in_next_layer[0]) {
                    all_the_same = false;
                    break;
                }
            }
            // just keep the first one if all the next nodes are the same
            if (all_the_same) {
                all_nodes_in_next_layer.erase(all_nodes_in_next_layer.begin() + 1, all_nodes_in_next_layer.end());
            }

            auto num_all_nodes_in_next_layer = all_nodes_in_next_layer.size();
            auto base_left = layer_base_left_cal(layer - 1);
            auto base_top = layer_base_top_cal(num_all_nodes_in_next_layer);

            auto index = 0;
            std::shared_ptr<NodeOnScreen> node_on_screen = nullptr;
            std::vector<std::shared_ptr<NodeOnScreen>> nodes_in_next_layer;
            for (int i = 0; i < nodes_in_layer.size(); i++) {
                auto node_left = base_left;
                auto next_nodes_in_pipe = nodes_in_layer[i]->get_orginal_node()->next_nodes();
                for (int j = 0; j < next_nodes_in_pipe.size(); j++) {
                    auto node_top = base_top + index * (node_height_ + node_gap_vertical_);
                    if (!all_the_same || node_on_screen == nullptr) {
                        node_on_screen = std::make_shared<NodeOnScreen>(next_nodes_in_pipe[j], tx_objects::Rect(node_left, node_top, node_width_, node_height_), layer);
                    }
                    nodes_in_layer[i]->get_next_nodes_on_screen().push_back(node_on_screen);

                    if (!all_the_same || nodes_in_next_layer.empty()) {
                        nodes_in_next_layer.push_back(node_on_screen);
                    }
                    index++;
                }
            }
            // next layer
            map_nodes(nodes_in_next_layer, layer + 1);
        } else {
            // cache the last layer
            des_nodes_on_screen = nodes_in_layer;
        } // recursion end
    }
}
} // namespace tx_utils
