#pragma once

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libswscale/swscale.h"
}
#include <iostream>
#include <memory>

/*
 * Safe libAVFormat
 * 使用智能指针管理 libAVFormat 的变量
 * 避免内存泄漏
 */

#define ASSERT_FFMPEG(FFMPEG_FUNC)                                                             \
    {                                                                                          \
        int CODE = FFMPEG_FUNC;                                                                \
        if (CODE == -11) {                                                                     \
            return false;                                                                      \
        }                                                                                      \
        if (CODE < 0) {                                                                        \
            char errbuf[1024];                                                                 \
            av_strerror(FFMPEG_FUNC, errbuf, sizeof(errbuf));                                  \
            std::cout << "\033[41m FFMPEG_FUNC error: \033[0m " << __FILE__ << ":" << __LINE__ \
                      << "\t" << errbuf << " " << CODE << std::endl;                           \
            return false;                                                                      \
        }                                                                                      \
    }
