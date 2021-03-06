//
// Created by sinochem on 2021/2/20.
//

#ifndef ITOP4412SIMPLE_H264_PUBLISH_H
#define ITOP4412SIMPLE_H264_PUBLISH_H

#include "video_publish.h"
#include "uvc_camera.h"

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

class H264Publisher : public VideoPublisher {
private:
    const char *outputPath;
    int width;
    int height;
    AVPacket avPacket;
    AVFormatContext *out_fmt = NULL;
    AVStream *pStream = NULL;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;
    uint8_t *pFrameBuffer = NULL;
    AVFrame *pFrame = NULL;

    // uvc摄像头相关
    UvcCamera *uvcCamera;
    AVFrame *yuyv422frame = NULL;
    uint8_t *yuyv422buf = NULL;
    struct SwsContext *sws_ctx;

    //AVFrame PTS
    int index = 0;

    int fps = 15;

    int InitYuyv422(int width, int height);

    int EncodeFrame(AVCodecContext *pCodecCtx, AVFrame *pFrame, AVPacket *pPkt);

public:
    void InitPublish(const char *outputPath, int width, int height);

    void StartPublish();

    void EncodeBuffer(unsigned char *nv21Buffer);

    void ToYuv420p(FrameCallback *fCb);

    void StopPublish();

    bool InitUvcSuccess();
};

#endif //ITOP4412SIMPLE_H264_PUBLISH_H
