/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_jiangyt_library_ffmpeg_FFmpegPlayer */

#ifndef _Included_com_jiangyt_library_ffmpeg_FFmpegPlayer
#define _Included_com_jiangyt_library_ffmpeg_FFmpegPlayer
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_jiangyt_library_ffmpeg_FFmpegPlayer
 * Method:    native_start
 * Signature: (Ljava/lang/String;Landroid/view/Surface;)V
 */
JNIEXPORT void JNICALL Java_com_jiangyt_library_ffmpeg_FFmpegPlayer_native_1start
        (JNIEnv *, jobject, jstring, jobject);
/*
 * Class:     com_jiangyt_library_ffmpeg_FFmpegPlayer
 * Method:    startPublish
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_ffmpeg_FFmpegPlayer_startPublish
        (JNIEnv *, jobject, jstring, jstring);
/*
 * Class:     com_jiangyt_library_ffmpeg_FFmpegPlayer
 * Method:    stopPublish
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_jiangyt_library_ffmpeg_FFmpegPlayer_stopPublish
        (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
