#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture capture;
    cv::Mat frame;

    capture.open("./data/rain001.mp4");
    cv::namedWindow("Rain", CV_WINDOW_AUTOSIZE);
    while (capture.read(frame)){
        cv::imshow("Rain", frame);
        cv::waitKey(10);
    }

    capture.release();
    return 0;
}