//
// Created by sinochem on 2020/10/12.
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
#include "itop_step_motor.h"

#undef TCSAFLUSH
#define TCSAFLUSH TCSETSF
#ifndef _TERMIOS_H_
#define _TERMIOS_H_
#endif

#define CMD_STEPMOTOR_A _IOW('L', 0, unsigned long)
#define CMD_STEPMOTOR_B _IOW('L', 1, unsigned long)
#define CMD_STEPMOTOR_C _IOW('L', 2, unsigned long)
#define CMD_STEPMOTOR_D _IOW('L', 3, unsigned long)

#define HIGHT 1
#define LOW 0
int fd = 0;
int flag = -1;

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopStepMotor
 * Method:    open
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopStepMotor_open
        (JNIEnv *env, jobject obj) {
    if (fd <= 0) {
        fd = open("/dev/step_motor_driver", O_RDWR | O_NDELAY);
    }
    if (fd <= 0) {
        __android_log_print(ANDROID_LOG_INFO, "StepMotor", "open /dev/step_motor_driver Error");
        flag = -1;
    } else {
        __android_log_print(ANDROID_LOG_INFO, "StepMotor",
                            "open /dev/step_motor_driver Success fd = %d", fd);
        flag = 0;
    }
    return flag;
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopStepMotor
 * Method:    close
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopStepMotor_close
        (JNIEnv *env, jobject obj) {
    if (fd > 0) close(fd);
    return 0;
}

/*
 * Class:     com_jiangyt_simple_itop_relay_ItopStepMotor
 * Method:    ioCtl
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_com_jiangyt_library_libitop_ItopStepMotor_ioCtl
        (JNIEnv *env, jobject obj, jint num, jint en) {
    switch (en) {
        case 0:
            ioctl(fd, CMD_STEPMOTOR_A, num);
            break;
        case 1:
            ioctl(fd, CMD_STEPMOTOR_B, num);
            break;
        case 2:
            ioctl(fd, CMD_STEPMOTOR_C, num);
            break;
        case 3:
            ioctl(fd, CMD_STEPMOTOR_D, num);
            break;
    }
    usleep(3);
    return 0;
}
