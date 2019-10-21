#include <iostream>
#include <opencv2/opencv.hpp>
#include <spalgorithm.h>
#include <videoprocess.h>
#include <Exceptions.h>
#include <configs.h>
#include <spprocess.h>
#include <stbuffer.h>
#include <colorspace.h>
#include <vector>

void replaceSP(const SuperPixel sp, const cv::Mat Xavg, cv::Mat I){
    for (int row=sp.row_start; row<sp.row_end; row++){
        for (int col=sp.col_start; col<sp.col_end; col++)
            if (sp.ILabel.at<int>(row, col) == sp.label)
                *(I.data + I.step[0]*row + I.step[1]*col) = \
                Xavg.at<uchar>(row-sp.row_start, col-sp.col_start);
    }
}

cv::Mat remove_rain(cv::Mat frame, SpatialTemporalBuffer video_buffer){
    cv::Mat YUVFrame, yChannel, uChannel, vChannel;
    /* Convert to YUV channel */
    cv::cvtColor(frame, YUVFrame, cv::COLOR_BGR2YCrCb);

    yChannel = getYUVChannel(YUVFrame, 0);
    uChannel = getYUVChannel(YUVFrame, 1);
    vChannel = getYUVChannel(YUVFrame, 2);

    cv::Mat yChannelRemoval = yChannel.clone();

    /* Get Object Edge */
    cv::Mat objEdge  = getObjectEdge(uChannel, vChannel, EPSILON_E);

    /* Super pixel segmentation result */
    std::vector<SuperPixel> SPs = SuperPixelSLIC(yChannel, SPSIZE);
    
    /* Get NS */
    int nxMax = 0;
    for (unsigned int i=0; i<SPs.size(); i++){
        nxMax = nxMax > SPs[i].row_end - SPs[i].row_start?\
        nxMax : SPs[i].row_end - SPs[i].row_start;
        nxMax = nxMax > SPs[i].col_end - SPs[i].col_start?\
        nxMax : SPs[i].col_end - SPs[i].col_start;
    }
    const int ns = nxMax * 2;

    SpatialTemporalBuffer video_bufferYUV(video_buffer.size());
    /* Conver video_buffer to YUV */
    for (int i=-video_buffer.size()+1; i<=0; i++)
    {
        cv::Mat vbi;
        cv::cvtColor(video_buffer[i], vbi, cv::COLOR_BGR2YCrCb);
        video_bufferYUV.update(vbi);
    }

    /* Process on each super pixel */
    for (unsigned int i=0; i<SPs.size(); i++){
        cv::Mat Xk  = getXk(SPs[i]);
        cv::Mat Msp = getMsp(SPs[i]);
        SubMatLocation BkPos = getBkPos(SPs[i], ns);
        SpatialTemporalBuffer Bk = video_bufferYUV.spatialAt(\
            BkPos.row_start,\
            BkPos.col_start,\
            BkPos.row_end,\
            BkPos.col_end,\
            0 // get Y channel in video_buffer
        );
        SpatialTemporalBuffer T0(video_buffer.size());
        for (int t=-video_buffer.size()+1; t<=0; t++){
            SubMatLocation T0iPos = getRefPos(Xk, Bk[t], Msp);
            T0.update(Bk[t].rowRange(T0iPos.row_start, T0iPos.row_end).\
                            colRange(T0iPos.col_start, T0iPos.col_end));
        }
        SpatialTemporalBuffer M0 = getM0(Xk, T0, EPSILON_RAIN);
        cv::Mat MrainT = getMrainT(M0, EPSILON_CONSRAIN);
        cv::Mat Me     = objEdge.rowRange(SPs[i].row_start, SPs[i].row_end).\
                                 colRange(SPs[i].col_start, SPs[i].col_end);
        cv::Mat Mrain  = MrainT.mul(1 - Me);
        cv::Mat Mrsp   = Msp.mul(1 - Mrain);
        cv::Mat Xavg   = getXavg(Bk, Xk, Mrsp);
        replaceSP(SPs[i], Xavg, yChannelRemoval);
    }

    cv::Mat frame_afterRemoval;
    cv::cvtColor(combineYUVChannel(yChannelRemoval, uChannel, vChannel),\
                 frame_afterRemoval, cv::COLOR_YCrCb2BGR);

    return frame_afterRemoval;
}

int main(int argc, char* argv[]){
    /* Read video file */
    Video videoRain(argv[1]);
    /* Set sample rate */
    videoRain.setInternal(1, VIDEO_INTERNAL_UNIT::FRAME);

    SpatialTemporalBuffer vbuffer(NT);

    cv::Mat currentFrame, frame_afterRemoval;
    int frame_cnt = 0;

    while (true){
        try{
            /* Read Next Frame */
            currentFrame = videoRain.nextFrame();
            std::cout<<"Currently processing "<<frame_cnt++<<" frame ..."<<std::endl;
            
            /* Modify the frame to proper size */
            cv::resize(currentFrame, currentFrame, cv::Size(WIDTH, HEIGHT));

            if (vbuffer.isFull())
            {
                frame_afterRemoval = remove_rain(currentFrame, vbuffer);

                cv::imshow("before", currentFrame);
                cv::imshow("after",  frame_afterRemoval);
                cv::waitKey(1000);
            }
            else
            {
                frame_afterRemoval = currentFrame;
            }    

            vbuffer.update(frame_afterRemoval);
        }
        catch(const int err){
            std::cout<<err<<std::endl;
            break;
        }
        cv::waitKey(1000);
    }

    return 0;
}