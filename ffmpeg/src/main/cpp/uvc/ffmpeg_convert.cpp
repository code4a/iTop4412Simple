//
// Created by sinochem on 2021/2/26.
//
#include "ffmpeg_convert.h"

void FFmpegConvert::init(int width, int height) {
    this->width = width;
    this->height = height;
}

uint8_t *FFmpegConvert::YUV2RGB(uint8_t *yuvBuf) {
    return smartConvert(yuvBuf, AV_PIX_FMT_YUYV422, AV_PIX_FMT_RGB565);
}

uint8_t *FFmpegConvert::YUYV2YUV420(uint8_t *yuvBuf) {
    return smartConvert(yuvBuf, AV_PIX_FMT_YUYV422, AV_PIX_FMT_YUV420P);
}

uint8_t *FFmpegConvert::smartConvert(uint8_t *srcBuf, enum AVPixelFormat src_pix_fmt,
                                     enum AVPixelFormat dst_pix_fmt) {
    AVFrame *srcFrame = av_frame_alloc();
    AVFrame *dstFrame = av_frame_alloc();
    struct SwsContext *sws_ctx = NULL;
    int dstBufSize = av_image_get_buffer_size(dst_pix_fmt, width, height, 1);
    uint8_t *dstBuf = (uint8_t *) av_malloc(dstBufSize);
    av_image_fill_arrays(srcFrame->data, srcFrame->linesize, srcBuf,
                         src_pix_fmt, width, height, 1);
    av_image_fill_arrays(dstFrame->data, dstFrame->linesize, dstBuf,
                         dst_pix_fmt, width, height, 1);
    sws_ctx = sws_getContext(width, height, src_pix_fmt, width, height,
                             src_pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
    sws_freeContext(sws_ctx);
    av_free(srcFrame);
    av_free(dstFrame);
    return dstBuf;
}
