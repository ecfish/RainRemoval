/*
 * Created by zhanghan, 2019.10.11
 */

#ifndef __VIDEO_PROCESS_H__
#define __VIDEO_PROCESS_H__

#include <opencv2/opencv.hpp>

namespace VIDEO_INTERNAL_UNIT{
    /* define the accepted video internal unit 
     * VIDEO_INTERNAL_UNIT::FRAME  -> internal measured in frame
     * VIDEO_INTERNAL_UNIT::SECOND -> internal measured in second
     */
    const int FRAME     = 0;
    const int SECOND    = 1;
}

class Video{
public:

    Video(const char* video_src);
    /* Create a video instance
     * @param video_src The video file path.
     */

    ~Video();

    void  setInternal(const unsigned int internal, const int unit);
    /* Set the internal between adjacent processing frames
     * @param internal The value of internal.
     * @param unit     The unit of internal. Availabel unit is defined in
     * namespace VIDEO_INTERNAL_UNIT. 
     * 
     * e.g. when call this function like:
     * setInternal(3, VIDEO_INTERNAL_UNIT::FRAME) means there are 3 frames
     * between adjacent processing frames.
     * setInternal(2, VIDEO_INTERNAL_UNIT::SECOND) means there are 2 seconds
     * between adjacent processing frames.
     * 
     * Note: If setInternal function is never called before processing,
     * internal is set to 1 frame as default.
     */

    cv::Mat nextFrame();
    /* Get the next frame content in video. The internal between current
     * frame and next frame is given by setInternal(internal, unit) function 
     * or set to 1 frame as default.
     */

    cv::Mat nextFrame(cv::Mat& frame);
    /* Similar to nextFrame(), but to save the next frame in the given
     * frame's address. 
     */

    cv::Mat reset();
    /* Not Implemented yet
     */

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