/*
 * Created by zhanghan, 2019.10.12
 */

#include <opencv2/opencv.hpp>
#include <configs.h>
#include <SPProcess.h>
#include <iostream>
#include <Exceptions.h>

void SPProcessor::load(cv::Mat *I, cv::Mat *ILabel, unsigned int numLabels){
    _ILabel = ILabel;
    _I = I;

    if (minX != NULL) free(minX);
    if (minY != NULL) free(minY);
    if (maxX != NULL) free(maxX);
    if (maxY != NULL) free(maxY);

    minX = (unsigned int *)malloc(sizeof(unsigned int)*numLabels);
    minY = (unsigned int *)malloc(sizeof(unsigned int)*numLabels);
    maxX = (unsigned int *)malloc(sizeof(unsigned int)*numLabels);
    maxY = (unsigned int *)malloc(sizeof(unsigned int)*numLabels);
    memset(minX, -1, sizeof(unsigned int)*numLabels);
    memset(minY, -1, sizeof(unsigned int)*numLabels);
    memset(maxX, 0,  sizeof(unsigned int)*numLabels);
    memset(maxY, 0,  sizeof(unsigned int)*numLabels);

    for (unsigned int i=0; i<(unsigned int)((*_ILabel).rows); i++){
        for (unsigned int j=0; j<(unsigned int)((*_ILabel).cols); j++)
        {
            int label = (*_ILabel).at<unsigned int>(i,j);
            *(minX + label) = *(minX + label) > j ? j:*(minX + label);
            *(maxX + label) = *(maxX + label) < j ? j:*(maxX + label);
            *(minY + label) = *(minY + label) > i ? i:*(minY + label);
            *(maxY + label) = *(maxY + label) < i ? i:*(maxY + label);
        }
    }
}

cv::Mat SPProcessor::getBoundingBoxXk(const unsigned int label){
    cv::Mat Xk = (*_I).rowRange(*(minY + label), *(maxY + label)).
                       colRange(*(minX + label), *(maxX + label));
    return Xk;
}

cv::Mat SPProcessor::getMsp(const unsigned int label){
    unsigned int sx = *(minX + label), ex = *(maxX + label);
    unsigned int sy = *(minY + label), ey = *(maxY + label);
    cv::Mat Msp(ey - sy + 1, ex - sx + 1, CV_8UC1);
    for (unsigned int i=sy; i<=ey; i++){
        for (unsigned int j=sx; j<=ex; j++)
        {
            if ((*_ILabel).at<unsigned int>(i,j) == label)
                *(Msp.data + Msp.step[0]*(i-sy) + Msp.step[1]*(j-sx)) = 1;
            else
                *(Msp.data + Msp.step[0]*(i-sy) + Msp.step[1]*(j-sx)) = 0;
        }
    }
    return Msp;
}

unsigned int* SPProcessor::getBkPos(const unsigned int label, const unsigned int ns){
    if (ns % 2 != 1)
        throw SPEXCEPTION::NS_NOT_ODD;
    if (ns > (unsigned int)((*_I).rows) || ns > (unsigned int)((*_I).cols))
        throw SPEXCEPTION::NS_TOO_BUG;
    
    unsigned int cx = (*(maxX + label) + *(minX + label))/2;
    unsigned int cy = (*(maxY + label) + *(minY + label))/2;
    unsigned int shift = (ns-1)/2;

    unsigned int* BkPos;
    BkPos = (unsigned int *)malloc(sizeof(unsigned int)*4);
    
    BkPos[0] = cx >= shift? cx - shift:0;
    BkPos[1] = cy >= shift? cy - shift:0;
    BkPos[2] = cx + cx - BkPos[0] <= (unsigned int)((*_I).cols-1)?\
               cx + cx - BkPos[0] : (unsigned int)((*_I).cols-1);
    BkPos[3] = cy + cy - BkPos[1] <= (unsigned int)((*_I).rows-1)?\
               cy + cy - BkPos[1] : (unsigned int)((*_I).rows-1);

    return BkPos;
}