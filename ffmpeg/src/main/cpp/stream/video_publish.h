//
// Created by sinochem on 2021/2/20.
//

#ifndef ITOP4412SIMPLE_VIDEO_PUBLISH_H
#define ITOP4412SIMPLE_VIDEO_PUBLISH_H

class VideoPublisher {

protected:

    bool transform = false;

public:

    virtual void InitPublish(const char *mp4Path, int width, int height) = 0;

    virtual void StartPublish() = 0;

    virtual void EncodeBuffer(unsigned char *nv21Buffer) = 0;

    virtual void StopPublish() = 0;

    bool isTransform();

};

#endif //ITOP4412SIMPLE_VIDEO_PUBLISH_H
