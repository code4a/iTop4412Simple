//
// Created by sinochem on 2021/2/26.
//
#ifndef ITOP4412SIMPLE_FFMPEG_CONVERT_H
#define ITOP4412SIMPLE_FFMPEG_CONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
#include "libavutil/time.h"

#ifdef __cplusplus
};
#endif

class FFmpegConvert {

private:
    int width;
    int height;

public:
    void init(int width, int height);

    uint8_t *YUV2RGB(uint8_t *yuvBuf);

    uint8_t *YUYV2YUV420(uint8_t *yuvBuf);

    uint8_t *smartConvert(uint8_t *srcBuf, enum AVPixelFormat src_pix_fmt,
                                 enum AVPixelFormat dst_pix_fmt);
};
#endif //ITOP4412SIMPLE_FFMPEG_CONVERT_H
