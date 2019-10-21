/*
 * Created by zhanghan, 2019.10.11
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <videoprocess.h>
#include <Exceptions.h>

Video::Video(const char* video_src){
    _videoCapture.open(video_src);
    if (!_videoCapture.isOpened()){
        throw VEXCEPTION::VIDEO_SRC_CAN_NOT_OPEN;
    }

    _frameHeight = (unsigned int)_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
    _frameWidth  = (unsigned int)_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    _fps         = (unsigned int)_videoCapture.get(cv::CAP_PROP_FPS);
    _numFrames   = (unsigned int)_videoCapture.get(cv::CAP_PROP_FRAME_COUNT);

    _frameStep   = 1; //frameStep is set to 1 default
}

void Video::setInternal(const unsigned int internal, const int unit){
    switch (unit)
    {
        case VIDEO_INTERNAL_UNIT::FRAME:
            _frameStep = internal;
            break;

        case VIDEO_INTERNAL_UNIT::SECOND:
            _frameStep = _fps * internal;
            break;
    
        default:
            throw VEXCEPTION::VIDEO_FRAME_INT_UNIT_UNKNOWN;
            break;
    }
}

Video::~Video(){
    _videoCapture.release();
}

cv::Mat Video::nextFrame(){
    unsigned int i = 0;
    for (; i<_frameStep && _videoCapture.read(_currentFrame); i++);
    if (i!=_frameStep){
        throw VEXCEPTION::NONE_REST_VIDEO_FRAME;
    }
    return _currentFrame.clone();
}

cv::Mat Video::nextFrame(cv::Mat& frame){
    unsigned int i = 0;
    for (; i<_frameStep && _videoCapture.read(frame); i++);
    if (i!=_frameStep){
        throw VEXCEPTION::NONE_REST_VIDEO_FRAME;
    }
    return frame;
}