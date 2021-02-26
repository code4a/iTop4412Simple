//
// Created by sinochem on 2021/2/20.
//

#ifndef ITOP4412SIMPLE_VIDEO_PUBLISH_H
#define ITOP4412SIMPLE_VIDEO_PUBLISH_H

#include <cstdint>

class VideoPublisher {

protected:

    bool transform = false;

public:

    virtual void InitPublish(const char *mp4Path, int width, int height) = 0;

    virtual void StartPublish() = 0;

    virtual void EncodeBuffer(unsigned char *nv21Buffer) = 0;

    virtual uint8_t *GetYuvBuf() = 0;

    virtual void ToYuv420p() = 0;

    virtual void StopPublish() = 0;

    bool isTransform();

    virtual bool InitUvcSuccess() = 0;
};

#endif //ITOP4412SIMPLE_VIDEO_PUBLISH_H
