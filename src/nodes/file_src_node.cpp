#include <iostream>

#include "utils/logger.h"
#include "nodes/file_src_node.h"

namespace tx_nodes {

FileSrcNode::FileSrcNode(std::string node_name,
                         int channel_index,
                         std::string file_path,
                         float resize_ratio,
                         bool cycle,
                         std::string gst_decoder_name,
                         int skip_interval) :
    SrcNode(node_name, channel_index, resize_ratio),
    file_path_(file_path),
    cycle_(cycle), gst_decoder_name_(gst_decoder_name), skip_interval_(skip_interval) {
    assert(skip_interval >= 0 && skip_interval <= 9);
    this->gst_template_ = tx_utils::string_format(this->gst_template_, file_path.c_str(), gst_decoder_name.c_str());
    TX_INFO(tx_utils::string_format("[%s] [%s]", node_name.c_str(), gst_template_.c_str()));
    this->initialized();
}

FileSrcNode::~FileSrcNode() {
    deinitialized();
}

// define how to read video from local file, create frame meta etc.
// please refer to the implementation of vp_node::handle_run.
void FileSrcNode::handle_run() {
    cv::Mat frame;
    int video_width = 0;
    int video_height = 0;
    int fps = 0;
    std::chrono::milliseconds delta;
    int skip = 0;

    while (alive_) {
        // check if need work
        gate_.knock();

        auto last_time = std::chrono::system_clock::now();
        // try to open capture
        if (!file_capture_.isOpened()) {
            if (!file_capture_.open(this->gst_template_, cv::CAP_GSTREAMER)) {
                TX_WARN(tx_utils::string_format("[%s] open file failed, try again...", node_name_.c_str()));
                continue;
            }
        }

        // video properties
        if (video_width == 0 || video_height == 0 || fps == 0) {
            video_width = file_capture_.get(cv::CAP_PROP_FRAME_WIDTH);
            video_height = file_capture_.get(cv::CAP_PROP_FRAME_HEIGHT);
            fps = file_capture_.get(cv::CAP_PROP_FPS);
            delta = std::chrono::milliseconds(1000 / fps) * (skip_interval_ + 1);

            original_fps_ = fps;
            original_width_ = video_width;
            original_height_ = video_height;

            // set true fps because skip some frames
            fps = fps / (skip_interval_ + 1);
        }
        // stream_info_hooker activated if need
        StreamInfo stream_info{channel_index_, original_fps_, original_width_, original_height_, to_string()};
        invoke_stream_info_hooker(node_name_, stream_info);

        file_capture_ >> frame;
        if (frame.empty()) {
            TX_INFO(tx_utils::string_format("[%s] reading frame complete, total frame==>%d", node_name_.c_str(), frame_index_));
            if (cycle_) {
                TX_INFO(tx_utils::string_format("[%s] cycle flag is true, continue!", node_name_.c_str()));
                file_capture_.set(cv::CAP_PROP_POS_FRAMES, 0);
            }
            continue;
        }

        // need skip
        if (skip < skip_interval_) {
            skip++;
            continue;
        }
        skip = 0;

        // need resize
        cv::Mat resize_frame;
        if (this->resize_ratio_ != 1.0f) {
            cv::resize(frame, resize_frame, cv::Size(), resize_ratio_, resize_ratio_);
        } else {
            resize_frame = frame.clone(); // clone!
        }
        // set true size because resize
        video_width = resize_frame.cols;
        video_height = resize_frame.rows;

        this->frame_index_++;
        // create frame meta
        auto out_meta =
            std::make_shared<tx_objects::FrameMeta>(resize_frame, this->frame_index_, this->channel_index_, video_width, video_height, fps);

        if (out_meta != nullptr) {
            this->out_queue_.push(out_meta);

            // handled hooker activated if need
            if (this->meta_handled_hooker) {
                meta_handled_hooker(node_name_, out_queue_.size(), out_meta);
            }

            // important! notify consumer of out_queue in case it is waiting.
            this->out_queue_semaphore_.signal();
            TX_DEBUG(tx_utils::string_format("[%s] after handling meta, out_queue.size()==>%d", node_name_.c_str(), out_queue_.size()));
        }

        // for fps
        auto snap = std::chrono::system_clock::now() - last_time;
        snap = std::chrono::duration_cast<std::chrono::milliseconds>(snap);
        if (snap < delta) {
            std::this_thread::sleep_for(delta - snap);
        }
    }

    // send dead flag for dispatch_thread
    this->out_queue_.push(nullptr);
    this->out_queue_semaphore_.signal();
}

// return stream path
std::string FileSrcNode::to_string() {
    return file_path_;
}
} // namespace tx_nodes
