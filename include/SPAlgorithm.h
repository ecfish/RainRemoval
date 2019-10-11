#ifndef __SPALGORITHM_H__
#define __SPALGORITHM_H__

#include <opencv2/opencv.hpp>

typedef struct {
    /*
     * The result format returned by superpixel algorithms
     */
    int pixelNums;      //The amount of superpixels

    cv::Mat* ILabel;    //The label map of the segmentation result
    cv::Mat* IMask;     /*The segmentation mask on input I, 
                         *you can use I.setTo((255, 255, 255), *(SPSegmentResult.IMask))
                         *to get the visual segmentation result
                         */

} SPSegmentResult;


SPSegmentResult SuperPixelSLIC(const cv::Mat I,\
                               int pixelSize  = 10,\
                               int iterations = 10,\
                               bool ifEnforceLabelConnectivity = true);
/*
 * SuperPixelSLIC uses SLIC method to segment the given image I into
 * superpixels. Currently, it uses the SLIC algorithm in opencv.
 * 
 * @param I             The given image
 * @param pixelSize     The superpixel size used in SLIC
 * @param iterations    Iterations for solving the segmentation result
 * @param ifEnforceLabelConnectivity    # need edited #
 */

#endif