//
// Created by sinochem on 2020/10/10.
//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <android/log.h>
#include "itop_relay.h"

#undef TCSAFLUSH
#define TCSAFLUSH TCSETSF
#ifndef _TERMIOS_H_
#define _TERMIOS_H_
#endif

int fd = 0;
int flag = -1;

/*
 * Class:     com_jiangyt_library_libitop_ItopRelay
 * Method:    open
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopRelay_open
        (JNIEnv *env, jobject obj) {
    if (fd <= 0) {
        fd = open("/dev/relay_ctl", O_RDWR | O_NDELAY | O_NOCTTY);
    }
    if (fd <= 0) {
        __android_log_print(ANDROID_LOG_INFO, "serial", "open /dev/relay_ctl Error");
        flag = -1;
    } else {
        __android_log_print(ANDROID_LOG_INFO, "serial", "open /dev/relay_ctl Success fd = %d", fd);
        flag = 0;
    }
    return flag;
}

/*
 * Class:     com_jiangyt_library_libitop_ItopRelay
 * Method:    close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopRelay_close
        (JNIEnv *env, jobject obj) {
    if (fd > 0) close(fd);
    return 0;
}

/*
 * Class:     com_jiangyt_library_libitop_ItopRelay
 * Method:    ioCtl
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopRelay_ioCtl
        (JNIEnv *env, jobject obj, jint num, jint en) {
    ioctl(fd, en, num);
    return 0;
}

