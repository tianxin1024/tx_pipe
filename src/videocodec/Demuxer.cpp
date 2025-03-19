#include "videocodec/Demuxer.h"
#include <chrono>
#include <thread>

namespace FFmpeg {

bool Demuxer::open(const std::string &url) {
    if (url.find("mp4") != std::string::npos) {
        m_suffix = "mp4";
    }

    avformat_network_init();
    av_dict_set(&opt, "buffer_size", "1024000", 0); //设置缓存大小,1080p可将值跳到最大
    av_dict_set(&opt, "rtsp_transport", "tcp", 0);  //以tcp的方式打开,
    av_dict_set(&opt, "stimeout", "5000000", 0);    //设置超时断开链接时间，单位us
    av_dict_set(&opt, "max_delay", "500000", 0);    //设置最大时延

    ASSERT_FFMPEG(avformat_open_input(&m_format_ctx, url.c_str(), nullptr, &opt));
    ASSERT_FFMPEG(avformat_find_stream_info(m_format_ctx, nullptr));

    // 查找视频流
    for (int i = 0; i < m_format_ctx->nb_streams; i++) {
        if (m_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            m_video_stream_index = i;
            break;
        }
    }
    if (m_video_stream_index == -1) {
        m_format_ctx = nullptr;
        return false;
    }
    av_dump_format(m_format_ctx, 0, url.c_str(), 0);
    return true;
}

} // namespace FFmpeg
