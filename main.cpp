#include <iostream>
#include <opencv2/opencv.hpp>
#include <SPAlgorithm.h>
#include <VideoProcess.h>
#include <Exceptions.h>

int main(int argc, char* argv[]){
    /* Read video file */
    Video videoRain(argv[1]);

    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
    while (true){
        try{
            cv::Mat currentFrame = videoRain.nextFrame();
            SPSegmentResult SPres = SuperPixelSLIC(currentFrame, 30);
            currentFrame.setTo((255, 255, 255), *(SPres.IMask));
            cv::imshow("Video", currentFrame);

        }
        catch(const int err){
            break;
        }
        cv::waitKey(10);
    }

    return 0;
}