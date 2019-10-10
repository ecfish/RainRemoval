/*
 * This file contains SuperPixel Algorithms used in Rain Removal
 * Created by HanZhang, 2019.10.10
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>

void SuperPixelSLIC(cv::Mat I){
    cv::Mat labels, mask;
    cv::namedWindow("SLIC", CV_WINDOW_AUTOSIZE);

    cv::Ptr <cv::ximgproc::SuperpixelSLIC> slic = cv::ximgproc::createSuperpixelSLIC(I);
    slic->iterate();//迭代次数，默认为10
    slic->enforceLabelConnectivity();
    slic->getLabelContourMask(mask);//获取超像素的边界
    slic->getLabels(labels);//获取labels
    int number = slic->getNumberOfSuperpixels();//获取超像素的数量

    I.setTo(cv::Scalar(255, 255, 255), mask);
    cv::imshow("SLIC", I);
    return;
}

int main(int argc, char* argv[]){
    cv::Mat img = cv::imread(argv[1]);
    SuperPixelSLIC(img);
    return 0;
}