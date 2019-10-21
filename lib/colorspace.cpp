/*
 * Created by zhanghan, 2019.10.20
 */

#include <opencv4/opencv2/opencv.hpp>

cv::Mat getYUVChannel(const cv::Mat I_YUV, const int channel){
    cv::Mat Ichannel(I_YUV.rows, I_YUV.cols, CV_8UC1);
    cv::mixChannels(I_YUV, Ichannel, new int[2]{channel, 0}, 1);
    return Ichannel;
}

cv::Mat combineYUVChannel(const cv::Mat IchannelY,\
                          const cv::Mat IchannelU,\
                          const cv::Mat IchannelV){
    cv::Mat combinedI(IchannelY.rows, IchannelY.cols, CV_8UC3);
    cv::mixChannels(IchannelY, combinedI, new int[2]{0, 0}, 1);
    cv::mixChannels(IchannelU, combinedI, new int[2]{0, 1}, 1);
    cv::mixChannels(IchannelV, combinedI, new int[2]{0, 2}, 1);
    return combinedI;
}
