#pragma once

#include <functional>
#include <mutex>
#include <string>

namespace tx_nodes {
// stream status created by des nodes.
struct StreamStatus {
    int channel_index = -1; // channel index
    int frame_index = -1;   // latest frame index
    int latency = 0;        // latency(ms) relative to src node

    float fps = 0;         // output fps of stream, maybe it is not equal to original_fps
    int width = 0;         // output width of stream, maybe it is not equal to original_width
    int height = 0;        // output height of stream, maybe it is not equal to original_height
    std::string direction; // where the stream goes to
};

// callback when stream is going out of pipe, happens in des nodes. MUST not be blocked.
typedef std::function<void(std::string, StreamStatus)> StreamStatusHooker;

// allow hookers attached to the pipe (des nodes specifically), hookers get notified when stream is going out of pipe.
// this class is inherited by vp_des_node only.
class StreamStatusHookable {
private:
    /* data */
protected:
    std::mutex stream_status_hooker_lock_;
    StreamStatusHooker stream_status_hooker_;

public:
    StreamStatusHookable(/* args */) {
    }
    ~StreamStatusHookable() {
    }

    void set_stream_status_hooker(StreamStatusHooker stream_status_hooker) {
        std::lock_guard<std::mutex> guard(stream_status_hooker_lock_);
        this->stream_status_hooker_ = stream_status_hooker;
    }

    void invoke_stream_status_hooker(std::string node_name, StreamStatus stream_status) {
        std::lock_guard<std::mutex> guard(stream_status_hooker_lock_);
        if (this->stream_status_hooker_) {
            this->stream_status_hooker_(node_name, stream_status);
        }
    }
};
} // namespace tx_nodes
