#pragma once

#include "nodes/node.h"
#include "nodes/stream_info_hookable.h"
#include "utils/invalid_error.h"
#include "utils/gate.h"

namespace tx_nodes {

// base class for src nodes, start point of meta/pipeline
class SrcNode : public Node, public StreamInfoHookable {
private:
    /* data */

protected:
    // force re-implemetation in child class
    virtual void handle_run() override;
    // force ignored in child class
    virtual std::shared_ptr<tx_objects::Meta> handle_frame_meta(std::shared_ptr<tx_objects::FrameMeta> meta) override;
    // force ignored in child class
    virtual std::shared_ptr<tx_objects::Meta> handle_control_meta(std::shared_ptr<tx_objects::ControlMeta> meta) override;

    // protected as it can't be instanstiated directly.
    SrcNode(std::string node_name,
            int channel_index,
            float resize_ratio = 1.0);

    // basic stream info
    int original_fps_ = -1;
    int original_width_ = 0;
    int original_height_ = 0;

    // basic channel info
    int frame_index_;
    int channel_index_;
    float resize_ratio_;

    // control to work or not
    // all derived class need depend on the value to check if work or not (start/stop)
    tx_utils::Gate gate_;

    // new logic for sending dead flag
    virtual void deinitialized() override;

public:
    ~SrcNode();

    virtual NodeType node_type() override;

    // start signal to pipeline
    void start();
    // stop signal to pipeline
    void stop();
    // speak signal to the pipeline (each node print some message such as current status)
    void speak();

    /* Debug API */
    // record video signal to pipeline, start recording video when vp_record_node(if exists) receive the signal.
    // it is a debug api since record signal is usually generated automatically inside pipe(not by users outside pipe).
    void record_video_manually(bool osd = false, int video_duration = 10);
    /* Debug API */
    // record image signal to pipeline, start recording image when vp_record_node(if exists) receive the signal.
    // it is a debug api since record signal is usually generated automatically inside pipe(not by userd outside pipe).
    void record_image_manually(bool osd = false);

    int get_original_fps() const;
    int get_original_width() const;
    int get_original_height() const;
};

} // namespace tx_nodes
