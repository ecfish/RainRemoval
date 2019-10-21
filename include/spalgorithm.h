/*
 * Created by zhanghan, 2019.10.11
 */
#ifndef __SPALGORITHM_H__
#define __SPALGORITHM_H__

#include <opencv2/opencv.hpp>
#include <spprocess.h>
#include <vector>

/*
 * SuperPixelSLIC uses SLIC method to segment the given image I into
 * superpixels. Currently, it uses the SLIC algorithm in opencv.
 * 
 * @param I             The given image
 * @param pixelSize     The superpixel size used in SLIC
 * @param iterations    Iterations for solving the segmentation result
 * @param ifEnforceLabelConnectivity    # need edited #
 */
std::vector<SuperPixel> SuperPixelSLIC(const cv::Mat I,\
                                       const int pixelSize  = 10,\
                                       const int iterations = 10,\
                                       const bool ifEnforceLabelConnectivity = true);

#endif