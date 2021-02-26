//
// Created by sinochem on 2021/2/23.
//
#include "uvc_camera.h"
#include "logger.h"

int UvcCamera::initCamera(int width, int height, int bufnum) {
    // 打开uvc摄像头
    cameraId = open(camera_name, O_RDWR);
    if (cameraId < 0)
        LOGE("%s ++++ open error\n", camera_name);
    int ret;
    this->width = width;
    this->height = height;
    this->bufnum = bufnum;
    // 设置摄像头格式和帧格式
    struct v4l2_format fmt;
    // 获取设备属性
    struct v4l2_capability cap;

    // 查询设备属性
    ret = ioctl(cameraId, VIDIOC_QUERYCAP, &cap);
    if (ret < 0) {
        LOGE("%d :VIDIOC_QUERYCAP failed\n", __LINE__);
        return -1;
    }
    // V4L2_CAP_VIDEO_CAPTURE // 是否支持图像获取
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        LOGE("%d : no capture devices\n", __LINE__);
        return -1;
    }
    // 将某一块内存中的内容全部设置为指定的值， 这个函数通常为新申请的内存做初始化工作。
    memset(&fmt, 0, sizeof(fmt));
    // 设置传输流类型
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    // 设置采样类型
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // yuv422
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED; // 采集区域
    // 设置当前格式为：视频捕捉格式
    if (ioctl(cameraId, VIDIOC_S_FMT, &fmt) < 0) {
        LOGE("++++%d : set format failed\n", __LINE__);
        return -1;
    }

    struct v4l2_requestbuffers req;
    req.count = bufnum;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(cameraId, VIDIOC_REQBUFS, &req);
    if (ret < 0) {
        LOGE("++++%d : VIDIOC_REQBUFS failed\n", __LINE__);
        return -1;
    }

    buffers = (fimc_buffer *) calloc(req.count, sizeof(*buffers));
    if (!buffers) {
        LOGE ("++++%d Out of memory\n", __LINE__);
        return -1;
    }
    // 初始化v4l2 buffer
    for (int i = 0; i < bufnum; ++i) {
        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2_buf.memory = V4L2_MEMORY_MMAP;
        v4l2_buf.index = i;
        // i 的缓冲区，得到其起始物理地址和大小
        ret = ioctl(cameraId, VIDIOC_QUERYBUF, &v4l2_buf);
        if (ret < 0) {
            LOGE("+++%d : VIDIOC_QUERYBUF failed\n", __LINE__);
            return -1;
        }
        buffers[i].length = v4l2_buf.length;
        if (MAP_FAILED == (buffers[i].start = (unsigned char *) mmap(0, v4l2_buf.length,
                                                                     PROT_READ | PROT_WRITE,
                                                                     MAP_SHARED,
                                                                     cameraId,
                                                                     v4l2_buf.m.offset))) {
            LOGE("%d : mmap() failed", __LINE__);
            return -1;
        }
        //映射内存
//        buffers[i].start = mmap(NULL, v4l2_buf.length, PROT_READ | PROT_WRITE, MAP_SHARED,
//                                cameraId, v4l2_buf.m.offset);
//        if (MAP_FAILED == buffers[i].start) {
//            LOGE("%d : mmap() failed", __LINE__);
//            return -1;
//        }
        // 把所需缓冲帧放入队列
        ret = ioctl(cameraId, VIDIOC_QBUF, &v4l2_buf);
        if (ret < 0) {
            LOGE("%d : VIDIOC_QBUF failed\n", __LINE__);
            return ret;
        }
    }
    initSuccess = true;
    return ret;
}

bool UvcCamera::InitSuccess() {
    return initSuccess;
}

int UvcCamera::streamOn() {
    int ret;
    // 把所需缓冲帧放入队列，并启动数据流
//    for (int i = 0; i < bufnum; ++i) {
//        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
//        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
//        v4l2_buf.memory = V4L2_MEMORY_MMAP;
//        v4l2_buf.index = i;
//        ret = ioctl(cameraId, VIDIOC_QBUF, &v4l2_buf);
//        if (ret < 0) {
//            LOGE("%d : VIDIOC_QBUF failed\n", __LINE__);
//            return ret;
//        }
//    }

    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(cameraId, VIDIOC_STREAMON, &type);
    if (ret < 0) {
        LOGE("%d : VIDIOC_STREAMON failed\n", __LINE__);
        return ret;
    }
    return ret;
}

int UvcCamera::dqbuf(uint8_t *data) {
    int ret;
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;

    ret = ioctl(cameraId, VIDIOC_DQBUF, &v4l2_buf);
    if (ret < 0) {
        LOGE("%s : VIDIOC_DQBUF failed, dropped frame\n", __func__);
        return ret;
    }
    memcpy(data, buffers[v4l2_buf.index].start, buffers[v4l2_buf.index].length);
    return v4l2_buf.index;
}

int UvcCamera::qbuf(int index) {
    int ret;
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
    v4l2_buf.index = index;
    ret = ioctl(cameraId, VIDIOC_QBUF, &v4l2_buf);
    if (ret < 0) {
        LOGE("%s : VIDIOC_QBUF failed\n", __func__);
        return ret;
    }
    return ret;
}

int UvcCamera::streamOff() {
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;
    // 停止数据流
    ret = ioctl(cameraId, VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
        LOGE("%s : VIDIOC_STREAMOFF failed\n", __func__);
        return ret;
    }
    return ret;
}

int UvcCamera::releaseCamera() {
//    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret;
//    ret = ioctl(cameraId, VIDIOC_STREAMOFF, &type);
//    if (ret < 0) {
//        LOGE("%s : VIDIOC_STREAMOFF failed\n", __func__);
//        return ret;
//    }

    for (int i = 0; i < bufnum; i++) {
        // 断开映射
        ret = munmap(buffers[i].start, buffers[i].length);
        if (ret < 0) {
            LOGE("%s : munmap failed\n", __func__);
            return ret;
        }
    }
    free(buffers);
    close(cameraId);
    initSuccess = false;
    return 0;
}
