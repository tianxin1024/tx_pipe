#include "utils/node_on_screen.h"

namespace tx_utils {

NodeOnScreen::NodeOnScreen(std::shared_ptr<tx_nodes::Node> original_node,
                           tx_objects::Rect node_rect,
                           int layer) :
    original_node_(original_node),
    node_rect_(node_rect),
    layer_(layer) {
    assert(original_node != nullptr);
    // register meta hookers for all nodes
    original_node->set_meta_arriving_hooker([this](std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        this->meta_arriving_hooker_storage.meta = meta;
        this->meta_arriving_hooker_storage.queue_size = queue_size;
        this->meta_arriving_hooker_storage.called_count_since_epoch_start++;
    });
    original_node->set_meta_handling_hooker([this](std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        this->meta_handling_hooker_storage.meta = meta;
        this->meta_handling_hooker_storage.queue_size = queue_size;
        this->meta_handling_hooker_storage.called_count_since_epoch_start++;
    });
    original_node->set_meta_handled_hooker([this](std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        this->meta_handled_hooker_storage.meta = meta;
        this->meta_handled_hooker_storage.queue_size = queue_size;
        this->meta_handled_hooker_storage.called_count_since_epoch_start++;
    });
    original_node->set_meta_leaving_hooker([this](std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        this->meta_leaving_hooker_storage.meta = meta;
        this->meta_leaving_hooker_storage.queue_size = queue_size;
        this->meta_leaving_hooker_storage.called_count_since_epoch_start++;
    });

    // register stream info hooker if it is a src node
    if (original_node->node_type() == tx_nodes::NodeType::SRC) {
        auto src_node = std::dynamic_pointer_cast<tx_nodes::SrcNode>(original_node);
        src_node->set_stream_info_hooker([this](std::string node_name, tx_nodes::StreamInfo stream_info) {
            this->stream_info_hooker_storage_ = stream_info;
        });
    }
    if (original_node->node_type() == tx_nodes::NodeType::DES) {
        auto des_node = std::dynamic_pointer_cast<tx_nodes::DesNode>(original_node);
        des_node->set_stream_status_hooker([this](std::string node_name, tx_nodes::StreamStatus stream_status) {
            this->stream_status_hooker_storage_ = stream_status;
        });
    }
}

NodeOnScreen::~NodeOnScreen() {
    // unregister meta hookers for all nodes
    original_node_->set_meta_arriving_hooker({});
    original_node_->set_meta_handling_hooker({});
    original_node_->set_meta_handled_hooker({});
    original_node_->set_meta_leaving_hooker({});

    // unregister stream info hooker if it is a src node
    if (original_node_->node_type() == tx_nodes::NodeType::SRC) {
        auto src_node = std::dynamic_pointer_cast<tx_nodes::SrcNode>(original_node_);
        src_node->set_stream_info_hooker({});
    }
    if (original_node_->node_type() == tx_nodes::NodeType::DES) {
        auto des_node = std::dynamic_pointer_cast<tx_nodes::DesNode>(original_node_);
        des_node->set_stream_status_hooker({});
    }
}

void NodeOnScreen::render_static_parts(cv::Mat &canvas) {
    auto node_left = node_rect_.x_;
    auto node_top = node_rect_.y_;
    auto node_width = node_rect_.width_;
    auto node_height = node_rect_.height_;

    cv::rectangle(canvas, cv::Rect(node_left, node_top, node_width, node_height), cv::Scalar(0, 0, 0), 1);
    // node_name
    tx_utils::put_text_at_center_of_rect(canvas, original_node_->node_name_, cv::Rect(node_left, node_top + 1, node_width, node_title_h_ - 2), false, font_face_);
    cv::line(canvas,
             cv::Point(node_left, node_top + node_title_h_),
             cv::Point(node_left + node_width - 1, node_top + node_title_h_),
             cv::Scalar(0, 0, 0), 1);

    // draw in_queue for non-src nodes
    if (original_node_->node_type() != tx_nodes::NodeType::SRC) {
        // connect line between in_queue and out_queue
        if (original_node_->node_type() == tx_nodes::NodeType::MID) {
            cv::line(canvas,
                     cv::Point(node_left + node_queue_width_ + node_queue_port_w_h_, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2),
                     cv::Point(node_left + node_width / 2, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2),
                     cv::Scalar(156, 156, 156), 1);
            cv::line(canvas,
                     cv::Point(node_left + node_width / 2, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2),
                     cv::Point(node_left + node_width / 2, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                     cv::Scalar(156, 156, 156), 1);
            cv::line(canvas,
                     cv::Point(node_left + node_width / 2, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                     cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                     cv::Scalar(156, 156, 156), 1);
            std::vector<cv::Point> vertexs{cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                                           cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_ * 2, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_),
                                           cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_ * 2, node_top + node_height - node_queue_port_padding_)};
            cv::fillPoly(canvas, std::vector<std::vector<cv::Point>>{vertexs}, cv::Scalar(156, 156, 156));
        } else {
            // DES
            cv::line(canvas,
                     cv::Point(node_left + node_queue_width_ + node_queue_port_w_h_, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2),
                     cv::Point(node_left + node_width / 2, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2),
                     cv::Scalar(156, 156, 156), 1);
            std::vector<cv::Point> vertexs{cv::Point(node_left + node_width / 2, node_top + node_title_h_ + node_queue_port_padding_),
                                           cv::Point(node_left + node_width / 2, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_),
                                           cv::Point(node_left + node_width / 2 + node_queue_port_w_h_, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2)};
            cv::fillPoly(canvas, std::vector<std::vector<cv::Point>>{vertexs}, cv::Scalar(156, 156, 156));
        }

        cv::line(canvas,
                 cv::Point(node_left + node_queue_width_, node_top + node_title_h_),
                 cv::Point(node_left + node_queue_width_, node_top + node_height - 1), cv::Scalar(0, 0, 0), 1);

        // in port
        cv::rectangle(canvas,
                      cv::Rect(node_left - node_queue_port_w_h_ + 1, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_, node_queue_port_w_h_, node_queue_port_w_h_),
                      cv::Scalar(156, 156, 156), 1);

        // out port
        cv::rectangle(canvas,
                      cv::Rect(node_left + node_queue_width_, node_top + node_title_h_ + node_queue_port_padding_, node_queue_port_w_h_, node_queue_port_w_h_),
                      cv::Scalar(156, 156, 156), 1);
    }
    // draw out_queue for non-des nodes
    if (original_node_->node_type() != tx_nodes::NodeType::DES) {
        // connect line between in_queue and out_queue
        if (original_node_->node_type() == tx_nodes::NodeType::SRC) {
            cv::line(canvas,
                     cv::Point(node_left + node_width / 2, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                     cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                     cv::Scalar(156, 156, 156), 1);

            std::vector<cv::Point> vertexs{cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                                           cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_ * 2, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_),
                                           cv::Point(node_left + node_width - node_queue_width_ - node_queue_port_w_h_ * 2, node_top + node_height - node_queue_port_padding_)};
            cv::fillPoly(canvas, std::vector<std::vector<cv::Point>>{vertexs}, cv::Scalar(156, 156, 156));
        }

        cv::line(canvas,
                 cv::Point(node_left + node_width - node_queue_width_, node_top + node_title_h_),
                 cv::Point(node_left + node_width - node_queue_width_, node_top + node_height - 1),
                 cv::Scalar(0, 0, 0), 1);

        // in port
        cv::rectangle(canvas,
                      cv::Rect(node_left + node_width - node_queue_width_ - node_queue_port_w_h_ + 1, node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_, node_queue_port_w_h_, node_queue_port_w_h_),
                      cv::Scalar(156, 156, 156), 1);
        // out port
        cv::rectangle(canvas,
                      cv::Rect(node_left + node_width - 1, node_top + node_title_h_ + node_queue_port_padding_, node_queue_port_w_h_, node_queue_port_w_h_),
                      cv::Scalar(156, 156, 156), 1);
    }

    // draw blocks connect line between nodes and nodes
    auto draw_connect_block = [=](int next_node_top) {                    
            cv::line(canvas, 
                        cv::Point(node_left + node_width + node_queue_port_w_h_, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2), 
                        cv::Point(node_left + node_width + node_gap_horizontal_ / 2, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2),
                        cv::Scalar(156, 156, 156), 1);
            cv::line(canvas, 
                    cv::Point(node_left + node_width + node_gap_horizontal_ / 2, node_top + node_title_h_ + node_queue_port_padding_ + node_queue_port_w_h_ / 2), 
                    cv::Point(node_left + node_width + node_gap_horizontal_ / 2, next_node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                    cv::Scalar(156, 156, 156), 1);
            cv::line(canvas, 
                    cv::Point(node_left + node_width + node_gap_horizontal_ / 2, next_node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2), 
                    cv::Point(node_left + node_width + node_gap_horizontal_ - node_queue_port_w_h_, next_node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2),
                    cv::Scalar(156, 156, 156), 1);

            std::vector<cv::Point> vertexs {cv::Point(node_left + node_width + node_gap_horizontal_ - node_queue_port_w_h_, next_node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_ / 2), 
                                            cv::Point(node_left + node_width + node_gap_horizontal_ - node_queue_port_w_h_ * 2, next_node_top + node_height - node_queue_port_padding_ - node_queue_port_w_h_), 
                                            cv::Point(node_left + node_width + node_gap_horizontal_ - node_queue_port_w_h_ * 2, next_node_top + node_height - node_queue_port_padding_)};
            cv::fillPoly(canvas, std::vector<std::vector<cv::Point>>{vertexs}, cv::Scalar(156, 156, 156)); };

    auto next_nodes_num = next_nodes_on_screen_.size();
    for (int j = 0; j < next_nodes_num; j++) {
        draw_connect_block(next_nodes_on_screen_[j]->node_rect_.y_);
    }
}

void NodeOnScreen::render_dynamic_parts(cv::Mat &canvas) {
    /*
        * draw data from hookers' callbacks
        */
    auto fps_func = [&](MetaHookerStorage &storage, cv::Rect rect) {
        auto called_count = storage.called_count_since_epoch_start;
        auto epoch_start = storage.time_epoch_start;
        auto delta_sec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - epoch_start);
        if (delta_sec.count() > fps_timeout_ * 1000 || (delta_sec.count() > fps_epoch_ && called_count > 0)) {
            //int fps = round(called_count * 1000.0 / delta_sec.count());
            auto fps = tx_utils::round_any(called_count * 1000.0 / delta_sec.count(), 1);
            storage.called_count_since_epoch_start = 0;
            storage.time_epoch_start = std::chrono::system_clock::now();
            storage.pre_fps = fps; // cache for next show
            tx_utils::put_text_at_center_of_rect(canvas,
                                                 fps,
                                                 rect, true,
                                                 font_face_, 1, cv::Scalar(26, 26, 139));
        } else {
            // use previous fps
            tx_utils::put_text_at_center_of_rect(canvas,
                                                 storage.pre_fps,
                                                 rect, true,
                                                 font_face_, 1, cv::Scalar(26, 26, 139));
        }
    };

    // non-src nodes
    if (original_node_->node_type() != tx_nodes::NodeType::SRC) {
        // size of in queue
        tx_utils::put_text_at_center_of_rect(canvas,
                                             std::to_string(std::max(meta_handling_hooker_storage.queue_size, 0)),
                                             cv::Rect(node_rect_.x_ + 3,
                                                      node_rect_.y_ + node_title_h_ / 2 + (node_rect_.height_ - node_title_h_) / 2, node_queue_width_ - 8, node_title_h_ - 10),
                                             true, font_face_, 1, cv::Scalar(255, 0, 255));
        // fps at 1st port
        fps_func(meta_arriving_hooker_storage, cv::Rect(node_rect_.x_ - node_queue_width_ / 2,
                                                        node_rect_.y_ + node_rect_.height_ - node_queue_port_padding_ * 3 - node_queue_port_w_h_ * 3 / 2,
                                                        node_queue_width_,
                                                        node_queue_port_padding_ + node_queue_port_w_h_));
        // fps at 2nd port
        fps_func(meta_handling_hooker_storage, cv::Rect(node_rect_.x_ + node_queue_width_ - node_queue_width_ / 2,
                                                        node_rect_.y_ + node_title_h_ + node_queue_port_padding_ * 3 / 2 + node_queue_port_w_h_,
                                                        node_queue_width_,
                                                        node_queue_port_padding_ + node_queue_port_w_h_));
    }

    // non-des nodes
    if (original_node_->node_type() != tx_nodes::NodeType::DES) {
        // size of out queue
        tx_utils::put_text_at_center_of_rect(canvas,
                                             std::to_string(std::max(meta_leaving_hooker_storage.queue_size, 0)),
                                             cv::Rect(node_rect_.x_ + node_rect_.width_ - node_queue_width_ + 3,
                                                      node_rect_.y_ + node_title_h_ / 2 + (node_rect_.height_ - node_title_h_) / 2, node_queue_width_ - 8, node_title_h_ - 10),
                                             true, font_face_, 1, cv::Scalar(255, 0, 255));
        // fps at 3rd port
        fps_func(meta_handled_hooker_storage, cv::Rect(node_rect_.x_ + node_rect_.width_ - node_queue_width_ - node_queue_width_ / 2,
                                                       node_rect_.y_ + node_rect_.height_ - node_queue_port_padding_ * 3 - node_queue_port_w_h_ * 3 / 2,
                                                       node_queue_width_,
                                                       node_queue_port_padding_ + node_queue_port_w_h_));
        // fps at 4th port
        fps_func(meta_leaving_hooker_storage, cv::Rect(node_rect_.x_ + node_rect_.width_ - node_queue_width_ / 2,
                                                       node_rect_.y_ + node_title_h_ + node_queue_port_padding_ * 3 / 2 + node_queue_port_w_h_,
                                                       node_queue_width_,
                                                       node_queue_port_padding_ + node_queue_port_w_h_));
    }

    auto node_left = node_rect_.x_;
    auto node_top = node_rect_.y_;
    // stream info at src nodes
    if (original_node_->node_type() == tx_nodes::NodeType::SRC) {
        tx_utils::put_text_at_center_of_rect(canvas, stream_info_hooker_storage_.uri,
                                             cv::Rect(node_left - node_rect_.width_ * 3 / 4, node_top + node_title_h_ + node_queue_port_padding_, node_rect_.width_ * 4 / 3, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
        tx_utils::put_text_at_center_of_rect(canvas, "original_width: " + std::to_string(stream_info_hooker_storage_.original_width),
                                             cv::Rect(node_left - node_rect_.width_ * 3 / 4, node_top + node_title_h_ * 5 / 3 + node_queue_port_padding_ * 2, node_rect_.width_ * 4 / 3, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
        tx_utils::put_text_at_center_of_rect(canvas, "original_height: " + std::to_string(stream_info_hooker_storage_.original_height),
                                             cv::Rect(node_left - node_rect_.width_ * 3 / 4, node_top + node_title_h_ * 7 / 3 + node_queue_port_padding_ * 3, node_rect_.width_ * 4 / 3, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
        tx_utils::put_text_at_center_of_rect(canvas, "original_fps: " + std::to_string(stream_info_hooker_storage_.original_fps),
                                             cv::Rect(node_left - node_rect_.width_ * 3 / 4, node_top + node_title_h_ * 9 / 3 + node_queue_port_padding_ * 4, node_rect_.width_ * 4 / 3, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
    }
    // stream status at des nodes
    if (original_node_->node_type() == tx_nodes::NodeType::DES) {
        tx_utils::put_text_at_center_of_rect(canvas, stream_status_hooker_storage_.direction,
                                             cv::Rect(node_left + node_rect_.width_ / 2 - 10, node_top + node_title_h_ * 5 / 3 + node_queue_port_padding_ * 2, node_rect_.width_ * 4 / 3 + 10, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
        tx_utils::put_text_at_center_of_rect(canvas, "output_fps: " + tx_utils::round_any(stream_status_hooker_storage_.fps, 2),
                                             cv::Rect(node_left + node_rect_.width_ / 2 - 10, node_top + node_title_h_ * 7 / 3 + node_queue_port_padding_ * 3, node_rect_.width_ * 4 / 3 + 10, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
        tx_utils::put_text_at_center_of_rect(canvas, "latency: " + std::to_string(stream_status_hooker_storage_.latency) + "ms",
                                             cv::Rect(node_left + node_rect_.width_ / 2 - 10, node_top + node_title_h_ * 9 / 3 + node_queue_port_padding_ * 4, node_rect_.width_ * 4 / 3 + 10, node_title_h_ * 2 / 3),
                                             true, font_face_, 1, cv::Scalar(), cv::Scalar(), cv::Scalar(255, 255, 255));
    }
}

std::shared_ptr<tx_nodes::Node> &NodeOnScreen::get_orginal_node() {
    return original_node_;
}

std::vector<std::shared_ptr<NodeOnScreen>> &NodeOnScreen::get_next_nodes_on_screen() {
    return next_nodes_on_screen_;
}
} // namespace tx_utils
