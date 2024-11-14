#pragma once

#include <functional>
#include <mutex>
#include <string>
#include <memory>

namespace tx_nodes {
// stream info created by src nodes.
struct StreamInfo {
    int channel_index = -1;  // channel index
    int original_fps = 0;    // original fps of stream
    int original_width = 0;  // width of resolution
    int original_height = 0; // height of resolution
    std::string uri = "";    // uri of stream
};

// callback when stream info changed, happens in src nodes. MUST not be blocked.
typedef std::function<void(std::string, StreamInfo)> StreamInfoHooker;

// allow hookers attached to the pipe (src nodes specifically), hookers get notified when stream info changed.
// this class is inherited by vp_src_node only.
class StreamInfoHookable {
private:
    /* data */
protected:
    std::mutex stream_info_hooker_lock_;
    StreamInfoHooker stream_info_hooker_;

public:
    StreamInfoHookable(/* args */) {
    }
    ~StreamInfoHookable() {
    }

    void set_stream_info_hooker(StreamInfoHooker stream_info_hooker) {
        std::lock_guard<std::mutex> guard(stream_info_hooker_lock_);
        this->stream_info_hooker_ = stream_info_hooker;
    }

    void invoke_stream_info_hooker(std::string node_name, StreamInfo stream_info) {
        std::lock_guard<std::mutex> guard(stream_info_hooker_lock_);
        if (this->stream_info_hooker_) {
            this->stream_info_hooker_(node_name, stream_info);
        }
    }
};
} // namespace tx_nodes
