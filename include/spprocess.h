/*
 * Created by zhanghan, 2019.10.20
 */

#ifndef __SPPROCESS_H__
#define __SPPROCESS_H__

#include <opencv4/opencv2/opencv.hpp>
#include <stbuffer.h>

struct SuperPixel{
    cv::Mat I, ILabel;

    int label;
    int row_start, row_end;
    int col_start, col_end;
};

struct SubMatLocation{
    int row_start, row_end;
    int col_start, col_end;
};

cv::Mat getXk(const SuperPixel sp);

cv::Mat getMsp(const SuperPixel sp);

SubMatLocation getBkPos(const SuperPixel sp, int ns);

SubMatLocation getRefPos(const cv::Mat Xk, const cv::Mat Bkt, const cv::Mat Msp);

SpatialTemporalBuffer getM0(const cv::Mat Xk,\
                            const SpatialTemporalBuffer T0,\
                            const int epsilon);

cv::Mat getMrainT(const SpatialTemporalBuffer M0, const int epsilon);

cv::Mat getObjectEdge(const cv::Mat IchannelU, const cv::Mat IchannelV, const int epsilon);

cv::Mat getXavg(const SpatialTemporalBuffer Bk, const cv::Mat Xk, const cv::Mat Mrsp);

#endif