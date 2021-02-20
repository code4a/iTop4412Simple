/* DO NOT EDIT THIS FILE - it is machine generated */
#include "ffmpeg_player.h"
#include <string>
#include <android/native_window_jni.h>
#include <unistd.h>
#include <android/log.h>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/avutil.h"
#include "libavutil/time.h"
#include "libavutil/imgutils.h"
#include "libavutil/frame.h"
}

#define FFLOGI(...) __android_log_print(ANDROID_LOG_INFO,"ffmpeg",##__VA_ARGS__);
#define FFLOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",##__VA_ARGS__);

/*
 * Class:     com_jiangyt_library_ffmpeg_FFmpegPlayer
 * Method:    native_start
 * Signature: (Ljava/lang/String;Landroid/view/Surface;)V
 */
extern "C" JNIEXPORT void JNICALL Java_com_jiangyt_library_ffmpeg_FFmpegPlayer_native_1start
        (JNIEnv *env, jobject instance, jstring absolutePath_, jobject surface) {
    // 获取渲染窗口
    ANativeWindow *aNativeWindow = ANativeWindow_fromSurface(env, surface);
    // FFMpeg 视频绘制
    const char *absolutePath = env->GetStringUTFChars(absolutePath_, 0);
    //1. 初始化网络模块
    avformat_network_init();
    //2. 获取视频流
    AVFormatContext *avFormatContext = avformat_alloc_context();
    AVDictionary *opts = NULL;
    av_dict_set(&opts, "timeout", "3000000", 0);
    int ret = avformat_open_input(&avFormatContext, absolutePath, NULL, &opts);
    if (ret) {
        return;
    }
    // 视频流索引
    int video_stream_index = -1;
    // 获取视频信息
    avformat_find_stream_info(avFormatContext, NULL);
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        // 判断如果是视频类型就赋值
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    // 获取解码器参数
    AVCodecParameters *avCodecParameters = avFormatContext->streams[video_stream_index]->codecpar;
    // 获取解码器
    AVCodec *avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
    // 解码器上下文
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    // 将解码器参数copy到解码器上下文
    avcodec_parameters_to_context(avCodecContext, avCodecParameters);
    // 打开解码器
    avcodec_open2(avCodecContext, avCodec, NULL);
    // 将YUV的数据转化为RGB数据
    SwsContext *swsContext = sws_getContext(avCodecContext->width, avCodecContext->height,
                                            avCodecContext->pix_fmt,
                                            avCodecContext->width, avCodecContext->height,
                                            AV_PIX_FMT_RGBA, SWS_BILINEAR, 0, 0, 0);
    // 设置ANativeWindow窗口缓存区
    ANativeWindow_setBuffersGeometry(aNativeWindow, avCodecContext->width, avCodecContext->height,
                                     WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer outBuffer;
    // 解码yuv数据
    AVPacket *avPacket = av_packet_alloc();
    // 从视频数据中获取数据包
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        avcodec_send_packet(avCodecContext, avPacket);
        AVFrame *avFrame = av_frame_alloc();
        int ret = avcodec_receive_frame(avCodecContext, avFrame);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        } else if (ret < 0) {
            break;
        }
        // 接收数据的容器
        uint8_t *dst_data[4];
        // 每一帧每一行的首地址
        int dst_linesize[4];
        av_image_alloc(dst_data, dst_linesize, avCodecContext->width, avCodecContext->height,
                       AV_PIX_FMT_RGBA, 1);
        // 绘制
        sws_scale(swsContext, avFrame->data, avFrame->linesize, 0, avFrame->height, dst_data,
                  dst_linesize);
        // 开始渲染，渲染前锁住窗口，渲染结束解锁窗口
        ANativeWindow_lock(aNativeWindow, &outBuffer, NULL);
        // 渲染
        uint8_t *firstWindow = static_cast<uint8_t *>(outBuffer.bits);
        // 输入源
        uint8_t *src_data = dst_data[0];
        // 一行有多少个字节RGBA
        int dstStride = outBuffer.stride * 4;
        int src_linesize = dst_linesize[0];
        for (int i = 0; i < outBuffer.height; ++i) {
            // 通过内存拷贝来进行渲染
            memcpy(firstWindow + i * dstStride, src_data + i * src_linesize, dstStride);
        }
        // 解锁
        ANativeWindow_unlockAndPost(aNativeWindow);
        usleep(1000 * 16);
        av_frame_free(&avFrame);
    }
    env->ReleaseStringUTFChars(absolutePath_, absolutePath);
}

//退出标记
int exit_flag = 1;

extern "C" JNIEXPORT jint JNICALL Java_com_jiangyt_library_ffmpeg_FFmpegPlayer_startPublish
        (JNIEnv *env, jobject instance, jstring mp4Path, jstring stream) {
    // string转char
    const char *absolutePath = env->GetStringUTFChars(mp4Path, 0);
    const char *streamUrl = env->GetStringUTFChars(stream, 0);
    //记录帧下标
    int frame_index = 0;
    //退出标记
    exit_flag = 1;
    // 视频流索引
    int video_stream_index = -1;
    AVOutputFormat *ofmt = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVFormatContext *in_fmt = NULL, *out_fmt = NULL;
    AVPacket avPacket;
    AVStream *in_stream = NULL, *out_stream = NULL;
    int64_t start_time;


    // 1. 注册所有组件
    av_register_all();
    // 2. 初始化网络
    avformat_network_init();
    // 3. 初始化接封装， 打开文件输入，获取视频流
    if (avformat_open_input(&in_fmt, absolutePath, 0, 0) < 0) {
        FFLOGE("Could not open input file.");
        goto end_line;
    }
    // 4. 查找相关流信息
    if (avformat_find_stream_info(in_fmt, 0) < 0) {
        FFLOGE("Failed to retrieve input stream information");
        goto end_line;
    }
    // 遍历视频轨
    for (int i = 0; i < in_fmt->nb_streams; i++) {
        // 判断如果是视频类型就赋值
        if (in_fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            video_stream_index = i;
            break;
        }
    }
    // 5. 初始化输出码流的AVFormatContext
    avformat_alloc_output_context2(&out_fmt, NULL, "flv", streamUrl); // RTMP
    if (!out_fmt) {
        FFLOGE("Could not create output context");
        goto end_line;
    }
    ofmt = out_fmt->oformat;
    // 遍历视频流
    for (int i = 0; i < in_fmt->nb_streams; i++) {
        // 6. 根据输入流创建一个输出流
        AVStream *in_stm = in_fmt->streams[i];
        codec_ctx = avcodec_alloc_context3(NULL);
        // 将解码器参数copy到解码器上下文
        avcodec_parameters_to_context(codec_ctx, in_stm->codecpar);
        AVStream *out_stm = avformat_new_stream(out_fmt, codec_ctx->codec);
        if (!out_stm) {
            FFLOGE("Failed allocating output stream");
            goto end_line;
        }
        codec_ctx->codec_tag = 0;
        if (out_fmt->oformat->flags & AVFMT_GLOBALHEADER) {
            codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }
        if (avcodec_parameters_from_context(out_stm->codecpar, codec_ctx) < 0) {
            goto end_line;
        }
    }
    // 7. 打开网络输出流
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        if (avio_open(&out_fmt->pb, streamUrl, AVIO_FLAG_WRITE) < 0) {
            FFLOGE("Could not open output URL '%s'", streamUrl);
            goto end_line;
        }
    }
    // 8. 写文件头部
    if (avformat_write_header(out_fmt, NULL) < 0) {
        FFLOGE("Error occurred when opening output URL");
        goto end_line;
    }

    // 记录开始时间
    start_time = av_gettime();
    // 读取帧数据
    while (exit_flag && av_read_frame(in_fmt, &avPacket) >= 0) {
        if (avPacket.stream_index == video_stream_index) {
            // 时间基
            AVRational time_base = in_fmt->streams[video_stream_index]->time_base;
            int64_t pts_time = av_rescale_q(avPacket.dts, time_base, AV_TIME_BASE_Q);
            int64_t now_time = av_gettime() - start_time;
            if (pts_time > now_time) {
                av_usleep(pts_time - now_time);
            }
        }
        in_stream = in_fmt->streams[avPacket.stream_index];
        out_stream = out_fmt->streams[avPacket.stream_index];

        //PTS主要用于度量解码后的视频帧什么时候被显示出来
        avPacket.pts = av_rescale_q_rnd(avPacket.pts, in_stream->time_base, out_stream->time_base,
                                        static_cast<AVRounding>(AV_ROUND_NEAR_INF |
                                                                AV_ROUND_PASS_MINMAX));
        //DTS主要是标识读入内存中的字节流在什么时候开始送入解码器中进行解码
        avPacket.dts = av_rescale_q_rnd(avPacket.dts, in_stream->time_base, out_stream->time_base,
                                        static_cast<AVRounding>(AV_ROUND_NEAR_INF |
                                                                AV_ROUND_PASS_MINMAX));
        avPacket.duration = av_rescale_q(avPacket.duration, in_stream->time_base,
                                         out_stream->time_base);
        avPacket.pos = -1;

        if (avPacket.stream_index == video_stream_index) {
            FFLOGI("Send %8d video frames to output URL", frame_index);
            frame_index++;
        }
        if (av_interleaved_write_frame(out_fmt, &avPacket) < 0) {
            FFLOGE("Error write frame");
            break;
        }
        av_packet_unref(&avPacket);
    }
    // 9. 收尾工作
    av_write_trailer(out_fmt);

    end_line:

    // 10. 关闭
    avformat_close_input(&in_fmt);
    if (out_fmt && !(ofmt->flags & AVFMT_NOFILE)) {
        avio_close(out_fmt->pb);
    }
    avformat_free_context(out_fmt);
    env->ReleaseStringUTFChars(mp4Path, absolutePath);
    env->ReleaseStringUTFChars(stream, streamUrl);
    return 0;
}

/**
 * 停止推流
 */
extern "C" JNIEXPORT void JNICALL Java_com_jiangyt_library_ffmpeg_FFmpegPlayer_stopPublish
        (JNIEnv *env, jobject instance) {
    exit_flag = 0;
}
