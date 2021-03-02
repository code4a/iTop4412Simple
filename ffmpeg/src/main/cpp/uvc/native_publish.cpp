//
// Created by sinochem on 2021/2/20.
//
#include <jni.h>

#include "native_publish.h"
#include "h264_publish.h"
#include "ffmpeg_convert.h"
#include "logger.h"
#include "callback.h"

/**
 * 动态注册
 */
JNINativeMethod methods[] = {
        {"setCallback",  "(Lcom/jiangyt/library/ffmpeg/FrameCallback;)I", (int *) setCallback},
        {"captureFrame", "()V",                                           (void *) captureFrame},
        {"startPublish", "(Ljava/lang/String;II)V",                       (void *) startPublish},
        {"stopPublish",  "()V",                                           (void *) stopPublish}
};

/**
 * 动态注册
 * @param env
 * @return
 */
jint registerNativeMethod(JNIEnv *env) {
    jclass cl = env->FindClass("com/jiangyt/library/ffmpeg/FFmpegUvcStream");
    if ((env->RegisterNatives(cl, methods, sizeof(methods) / sizeof(methods[0]))) < 0) {
        return -1;
    }
    return 0;
}

/**
 * 加载默认回调
 * @param vm
 * @param reserved
 * @return
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    //注册方法
    if (registerNativeMethod(env) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}

jobject frameCallback = NULL;
jclass cls = NULL;
jmethodID mid = NULL;
VideoPublisher *videoPublisher = NULL;
FFmpegConvert *fmpegConvert = NULL;

int callback(JNIEnv *env, uint8_t *rgbBuf, uint8_t *yuvBuf) {
    if (frameCallback == NULL) {
        return -3;
    }
    if (mid == NULL) {
        return -2;
    }
    if (cls == NULL) {
        return -1;
    }
    jbyteArray rgb = env->NewByteArray(sizeof(rgbBuf));
    env->SetByteArrayRegion(rgb, 0, sizeof(rgbBuf), (jbyte *) rgbBuf);
    jbyteArray yuv = env->NewByteArray(sizeof(yuvBuf));
    env->SetByteArrayRegion(yuv, 0, sizeof(yuvBuf), (jbyte *) yuvBuf);
    env->CallVoidMethod(frameCallback, mid, rgb, yuv);
    env->DeleteLocalRef(rgb);
    env->DeleteLocalRef(yuv);
    return 0;
}

void FrameCallback::initJni(JNIEnv *env) {
    this->env = env;
}

void FrameCallback::frameCall(uint8_t *yuvData) {
    LOGE("callback yuv data size : %d", (int)sizeof(yuvData));
    int ret = callback(env, fmpegConvert->YUV2RGB(yuvData),
                       fmpegConvert->YUYV2YUV420(yuvData));
    LOGE("callback ret : %d", ret);
}

int setCallback(JNIEnv *env, jobject instance, jobject frameCp) {
    // 转换为全局变量
    frameCallback = env->NewGlobalRef(frameCp);
    if (frameCallback == NULL) {
        return -3;
    }
    cls = env->GetObjectClass(frameCallback);
    if (cls == NULL) {
        return -1;
    }
    mid = env->GetMethodID(cls, "frameCallback", "([B[B)V");
    if (mid == NULL) {
        return -2;
    }
    env->CallVoidMethod(frameCallback, mid, (jbyteArray) nullptr, (jbyteArray) nullptr);
    LOGE("set callback sucess");
    return 0;
}

/**
 * 编码开始
 * @param env
 * @param obj
 * @param jmp4Path
 * @param width
 * @param height
 */
void startPublish(JNIEnv *env, jobject obj, jstring jmp4Path, jint width, jint height) {
    const char *mp4Path = env->GetStringUTFChars(jmp4Path, NULL);
    if (videoPublisher == NULL) {
        videoPublisher = new H264Publisher();
    }
    if (fmpegConvert == NULL) {
        fmpegConvert = new FFmpegConvert();
        fmpegConvert->init(width, height);
    }
    videoPublisher->InitPublish(mp4Path, width, height);
    videoPublisher->StartPublish();

    env->ReleaseStringUTFChars(jmp4Path, mp4Path);
}

/**
 * 编码结束
 * @param env
 * @param obj
 * @param jmp4Path
 * @param width
 * @param height
 */
void stopPublish(JNIEnv *env, jobject obj) {
    if (NULL != videoPublisher) {
        videoPublisher->StopPublish();
        videoPublisher = NULL;
    }
}

/**
 * 打开usb摄像头
 * @param env
 * @param obj
 */
void captureFrame(JNIEnv *env, jobject obj) {
    FrameCallback *fCb = new FrameCallback();
    fCb->initJni(env);
    while (NULL != videoPublisher && videoPublisher->isTransform()) {
        if (videoPublisher->InitUvcSuccess()) {
            videoPublisher->ToYuv420p(fCb);
        } else {
            videoPublisher->EncodeBuffer(NULL);
        }

    }
}
