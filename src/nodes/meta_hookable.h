#pragma once
#include <functional>
#include <mutex>
#include <string>
#include <memory>

#include "objects/meta.h"

namespace tx_nodes {

// callback when meta flowing through the whole pipe, MUST NOT be blocked.
// we can do more work based on this callback, such as calculating fps/latency at each port of node, please refer to vp_analysis_board for details.
typedef std::function<void(std::string, int, std::shared_ptr<tx_objects::Meta>)> meta_hooker;

// allow hookers attached to the pipe (nodes), get notified when meta flow through each port of node (total 4 ports in node).
// this class is inherited by tx_node only.
class MetaHookable {
protected:
    std::mutex meta_arriving_hooker_lock_;
    std::mutex meta_handling_hooker_lock_;
    std::mutex meta_handled_hooker_lock_;
    std::mutex meta_leaving_hooker_lock_;
    // hooker activated when meta is arriving at node (pushed to in_queue of vp_node, the 1st port in node).
    meta_hooker meta_arriving_hooker_;
    // hooker activated when meta is to be handled inside node (poped from in_queue of vp_node, the 2nd port in node).
    meta_hooker meta_handling_hooker_;
    // hooker activated when meta is handled inside node (pushed to out_queue of vp_node, the 3rd port in node).
    meta_hooker meta_handled_hooker_;
    // hooker activated when meta is leaving from node (poped from out_queue of vp_node, the 4th port in node).
    meta_hooker meta_leaving_hooker_;

public:
    MetaHookable(/* args */) {
    }
    ~MetaHookable() {
    }

    void set_meta_arriving_hooker(meta_hooker meta_arriving_hooker) {
        std::lock_guard<std::mutex> guard(meta_arriving_hooker_lock_);
        this->meta_arriving_hooker_ = meta_arriving_hooker;
    }

    void set_meta_handling_hooker(meta_hooker meta_handling_hooker) {
        std::lock_guard<std::mutex> guard(meta_handling_hooker_lock_);
        this->meta_handling_hooker_ = meta_handling_hooker;
    }

    void set_meta_handled_hooker(meta_hooker meta_handled_hooker) {
        std::lock_guard<std::mutex> guard(meta_handled_hooker_lock_);
        this->meta_handled_hooker_ = meta_handled_hooker;
    }

    void set_meta_leaving_hooker(meta_hooker meta_leaving_hooker) {
        std::lock_guard<std::mutex> guard(meta_leaving_hooker_lock_);
        this->meta_leaving_hooker_ = meta_leaving_hooker;
    }

    void invoke_meta_arriving_hooker(std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        std::lock_guard<std::mutex> guard(meta_arriving_hooker_lock_);
        if (this->meta_arriving_hooker_) {
            this->meta_arriving_hooker_(node_name, queue_size, meta);
        }
    }

    void invoke_meta_handling_hooker(std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        std::lock_guard<std::mutex> guard(meta_handling_hooker_lock_);
        if (this->meta_handling_hooker_) {
            this->meta_handling_hooker_(node_name, queue_size, meta);
        }
    }

    void invoke_meta_handled_hooker(std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        std::lock_guard<std::mutex> guard(meta_handled_hooker_lock_);
        if (this->meta_handled_hooker_) {
            this->meta_handled_hooker_(node_name, queue_size, meta);
        }
    }

    void invoke_meta_leaving_hooker(std::string node_name, int queue_size, std::shared_ptr<tx_objects::Meta> meta) {
        std::lock_guard<std::mutex> guard(meta_leaving_hooker_lock_);
        if (this->meta_leaving_hooker_) {
            this->meta_leaving_hooker_(node_name, queue_size, meta);
        }
    }
};
} // namespace tx_nodes
