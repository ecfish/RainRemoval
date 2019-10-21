/*
 * Created by zhanghan, 2019.10.20
 */

#ifndef __COLORSPACE_H__
#define __COLORSPACE_H__

#include <opencv4/opencv2/opencv.hpp>

cv::Mat getYUVChannel(const cv::Mat I_YUV, const int channel);

cv::Mat combineYUVChannel(const cv::Mat IchannelY,\
                          const cv::Mat IchannelU,\
                          const cv::Mat IchannelV);

#endif