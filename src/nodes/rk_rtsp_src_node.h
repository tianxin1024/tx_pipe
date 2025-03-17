#pragma once

#include <string>
#include "nodes/src_node.h"
#include "videocodec/Demuxer.h"

namespace tx_nodes {

// rtsp source node, receive video stream via rtsp protocal.
// example:
// rtsp://localhost:8554/stream
class RkRtspSrcNode : public SrcNode {
private:
    std::shared_ptr<FFmpeg::Demuxer> m_demux; // 解封装
    std::string rtsp_url;
    int skip_interval = 0;
    int step = 0;
    // 读取的视频信息
    int m_width, m_height, m_fps = 0;
    // bool init();

protected:
    // re-implementation
    virtual void handle_run() override;

public:
    RkRtspSrcNode(std::string node_name,
                  int channel_index,
                  std::string rtsp_url,
                  float resize_ratio = 1.0,
                  int skip_interval = 0);
    ~RkRtspSrcNode();

    virtual std::string to_string() override;
};

} // namespace tx_nodes
