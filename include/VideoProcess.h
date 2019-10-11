/*
 * Created by zhanghan, 2019.10.11
 */

#ifndef __VIDEO_PROCESS_H__
#define __VIDEO_PROCESS_H__

#include <opencv2/opencv.hpp>

namespace VIDEO_INTERNAL_UNIT{

    const int FRAME     = 0;
    const int SECOND    = 1;

}

class Video{
public:
    Video(const char* video_src);

    ~Video();

    void  setInternal(const unsigned int internal, const int unit);
    cv::Mat nextFrame();
    cv::Mat reset();

private:
    cv::VideoCapture _videoCapture;
    cv::Mat _currentFrame;

    unsigned int _frameHeight;
    unsigned int _frameWidth;

    unsigned int _fps;
    unsigned int _numFrames;

    unsigned int _frameStep;
};

#endif