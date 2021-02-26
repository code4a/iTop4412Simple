//
// Created by sinochem on 2021/2/23.
//

#ifndef ITOP4412SIMPLE_UVC_DEVICE_H
#define ITOP4412SIMPLE_UVC_DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavdevice/avdevice.h"

#ifdef __cplusplus
};
#endif

class UvcDevice {
protected:
    const char *input_name = "video4linux2";
    const char *uvc_name = "/dev/video4";

    AVFormatContext *fmtCtx = NULL;
    AVInputFormat *inputFmt;

public:
    int openUvcDevice();

    int captureFrame(AVPacket *packet);

    int releaseRes();
};

#endif //ITOP4412SIMPLE_UVC_DEVICE_H
