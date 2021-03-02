//
// Created by sinochem on 2021/3/1.
//

#ifndef ITOP4412SIMPLE_CALLBACK_H
#define ITOP4412SIMPLE_CALLBACK_H

#include <cstdint>
#include <jni.h>

class FrameCallback {
protected:
    JNIEnv *env;
public:
    void initJni(JNIEnv *env);

    void frameCall(uint8_t *yuvData);
};

#endif //ITOP4412SIMPLE_CALLBACK_H
