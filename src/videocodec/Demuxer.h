#pragma once

#include "videocodec/SafeAVFormat.h"

namespace FFmpeg {
class Demuxer {
public:
    Demuxer() = default;
    ~Demuxer();

    bool open(const std::string &url);
    void close();

private:
    AVDictionary *opt = nullptr;
    AVFormatContext *m_format_ctx = nullptr;
    int m_video_stream_index = -1;
    std::string m_suffix;
};

} // namespace FFmpeg
