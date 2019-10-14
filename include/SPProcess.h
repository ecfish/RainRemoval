/* 
 * Created by zhanghan, 2019.10.12
 */

#include <opencv2/opencv.hpp>
#include <configs.h>

class SPProcessor{
public:
    void load(cv::Mat* I, cv::Mat *ILabel, unsigned int numLabels);
    cv::Mat getBoundingBoxXk(const unsigned int label);
    cv::Mat getMsp(const unsigned int label);
    unsigned int* getBkPos(const unsigned int label, const unsigned int ns=NS);
private:
    cv::Mat *_ILabel;
    cv::Mat *_I;

    unsigned int *minX = NULL;
    unsigned int *maxX = NULL;
    unsigned int *minY = NULL;
    unsigned int *maxY = NULL;
};