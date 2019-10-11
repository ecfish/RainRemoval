/*
 * This file contains SuperPixel Algorithms used in Rain Removal
 * Created by HanZhang, 2019.10.10
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <SPAlgorithm.h>

SPSegmentResult SuperPixelSLIC(const cv::Mat I, int pixelSize, int iterations, bool ifEnforceLabelConnectivity){
    SPSegmentResult ret;
    
    ret.IMask  = new cv::Mat();
    ret.ILabel = new cv::Mat();

    cv::Ptr <cv::ximgproc::SuperpixelSLIC> slic = \
    cv::ximgproc::createSuperpixelSLIC(I, cv::ximgproc::SLICO, pixelSize);
    
    slic->iterate(iterations);
    if (ifEnforceLabelConnectivity)
        slic->enforceLabelConnectivity();

    slic->getLabelContourMask(*(ret.IMask));
    slic->getLabels(*(ret.ILabel));
    ret.pixelNums = slic->getNumberOfSuperpixels();
    
    return ret;
}