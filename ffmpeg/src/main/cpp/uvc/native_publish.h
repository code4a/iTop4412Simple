//
// Created by sinochem on 2021/2/20.
//
#include <jni.h>
/* Header for class com_jiangyt_library_ffmpeg_FFmpegStream */

#ifndef INCLUDED_COM_JIANGYT_LIBRARY_FFMPEG_FFMPEGSTREAM
#define INCLUDED_COM_JIANGYT_LIBRARY_FFMPEG_FFMPEGSTREAM
#ifdef __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"

/*
* Class:     com_jiangyt_library_ffmpeg_FFMpegUvcStream
* Method:    setCallback
* Signature: (Lcom/jinmao/cloud/app/ffmpeg/FrameCallback;)I
*/
JNIEXPORT jint JNICALL setCallback(JNIEnv *, jobject, jobject);

/*
 * Class:     com_jiangyt_library_ffmpeg_FFmpegUvcStream
 * Method:    captureFrame
 * Signature: ()V
 */
JNIEXPORT void JNICALL captureFrame(JNIEnv *, jobject);

JNIEXPORT void JNICALL startPublish(JNIEnv *, jobject, jstring, jint, jint);

JNIEXPORT void JNICALL stopPublish(JNIEnv *, jobject);


#ifdef __cplusplus
}
#endif
#endif