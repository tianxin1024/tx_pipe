#pragma once

#include <string>

#include "nodes/node.h"
#include "nodes/stream_status_hookable.h"

namespace tx_nodes {

// base class for des nodes, end point of meta/pipeline.
class DesNode : public Node, public StreamStatusHookable {
private:
    // cache for stream status at current des node.
    StreamStatus stream_status_;

    // period(ms) to calculate output fps
    int fps_epoch_ = 500;
    int fps_counter_ = 0;
    std::chrono::system_clock::time_point fps_last_time_;

protected:
    // do nothing in des nodes
    virtual void dispatch_run() override final;
    // sample implementation, return nullptr in all des nodes.
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) override;
    // sample implementation, return nullptr in all des nodes.
    virtual std::shared_ptr<tx_objects::Meta> handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) override;

    // protected as it can't be instanstiated directly.
    DesNode(std::string node_name, int channel_index);

public:
    ~DesNode();

    virtual NodeType node_type() override;

    int channel_index_;
};
} // namespace tx_nodes
