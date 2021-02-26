#include "uvc_device.h"
#include "logger.h"

//
// Created by sinochem on 2021/2/23.
//
int UvcDevice::openUvcDevice() {
    inputFmt = av_find_input_format(input_name);
    if (inputFmt == NULL) {
        LOGE("can not find_input_format\n");
        return -1;
    }
    int ret = avformat_open_input(&fmtCtx, uvc_name, inputFmt, NULL);
    if (ret < 0) {
        LOGE("can not open_input_file\n");
        return ret;
    }
    /* print device information*/
    //av_dump_format(fmtCtx, 0, uvc_name, 0);
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    av_read_frame(fmtCtx, packet);
    LOGE("data length = %d\n",packet->size);
}

int UvcDevice::captureFrame(AVPacket *packet) {

    av_read_frame(fmtCtx, packet);
    LOGI("data length = %d\n", packet->size);
    return 0;
}

int UvcDevice::releaseRes() {
    avformat_close_input(&fmtCtx);
}
