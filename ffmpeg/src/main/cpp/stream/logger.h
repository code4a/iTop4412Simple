//
// Created by sinochem on 2021/2/20.
//

#ifndef ITOP4412SIMPLE_LOGGER_H
#define ITOP4412SIMPLE_LOGGER_H

#ifdef ANDROID

#include <android/log.h>
#include <libavutil/time.h>

#define LOG_TAG    "NativeEncode"
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf(LOG_TAG format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf(LOG_TAG format "\n", ##__VA_ARGS__)
#endif

#endif //ITOP4412SIMPLE_LOGGER_H
