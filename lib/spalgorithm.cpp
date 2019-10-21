/*
 * This file contains SuperPixel Algorithms used in Rain Removal
 * Created by HanZhang, 2019.10.10
 */

#include <vector>
#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/ximgproc.hpp>
#include <spalgorithm.h>
#include <spprocess.h>

std::vector<SuperPixel> SuperPixelSLIC(const cv::Mat I,\
                                       const int pixelSize,\
                                       const int iterations,\
                                       const bool ifEnforceLabelConnectivity){
    std::vector<SuperPixel> ret;
    cv::Mat ILabel;

    cv::Ptr <cv::ximgproc::SuperpixelSLIC> slic = \
    cv::ximgproc::createSuperpixelSLIC(I, cv::ximgproc::SLICO, pixelSize);
    
    slic->iterate(iterations);
    if (ifEnforceLabelConnectivity)
        slic->enforceLabelConnectivity();
    slic->getLabels(ILabel);
    
    ret.resize(slic->getNumberOfSuperpixels());
    for (unsigned int i=0; i<ret.size(); i++){
        ret[i].row_start = ret[i].col_start = 0x7FFFFFFF;
        ret[i].row_end   = ret[i].col_end   = 0;
        ret[i].I         = I;
        ret[i].ILabel    = ILabel;
        ret[i].label     = i;
    }

    for (int row=0; row<I.rows; row++){
        for (int col=0; col<I.cols; col++)
        {
            int label = ILabel.at<int>(row, col);
            ret[label].row_start = ret[label].row_start > row? row:ret[label].row_start;
            ret[label].row_end   = ret[label].row_end   > row? ret[label].row_end:row;
            ret[label].col_start = ret[label].col_start > col? col:ret[label].col_start;
            ret[label].col_end   = ret[label].col_end   > col? ret[label].col_end:col;
        }
    }

    return ret;
}