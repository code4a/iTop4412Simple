//
// Created by sinochem on 2021/2/23.
//

#ifndef ITOP4412SIMPLE_UVC_CAMERA_H
#define ITOP4412SIMPLE_UVC_CAMERA_H

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/videodev2.h>

struct fimc_buffer {
    unsigned char *start;
    size_t length;
};

class UvcCamera {
protected:
    const char *camera_name = "/dev/video4";
    struct fimc_buffer *buffers = NULL;
    struct v4l2_buffer v4l2_buf;
    int width;
    int height;
    int bufnum = 1;
    int cameraId = -1;

    bool initSuccess = false;
public:

    /**
     * 初始化uvc摄像头
     * @param width 宽度
     * @param height 高度
     * @param bufnum 缓存长度
     * @return 结果
     */
    int initCamera(int width, int height, int bufnum);

    /**
     * 开始视频流
     * @return 结果
     */
    int streamOn();

    /**
     * 从队列中取出帧，并拷贝到data中
     * @param data 数据
     * @return 结果
     */
    int dqbuf(uint8_t *data);

    /**
     * 把帧放入队列
     * @return 结果
     */
    int qbuf(int index);

    /**
     * 关闭流
     * @return 结果
     */
    int streamOff();

    /**
     * 释放摄像头相关资源
     * @return 结果
     */
    int releaseCamera();

    /**
     * 摄像头是否初始化成功
     */
    bool InitSuccess();
};

#endif //ITOP4412SIMPLE_UVC_CAMERA_H
