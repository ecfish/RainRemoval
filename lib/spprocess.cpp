/*
 * Created by zhanghan, 2019.10.19
 */

#include <opencv4/opencv2/opencv.hpp>
#include <spprocess.h>
#include <stbuffer.h>

cv::Mat getXk(const SuperPixel sp){
    return sp.I.rowRange(sp.row_start, sp.row_end).\
                colRange(sp.col_start, sp.col_end);
}

cv::Mat getMsp(const SuperPixel sp){
    cv::Mat Msp(sp.row_end - sp.row_start,\
                sp.col_end - sp.col_start,\
                CV_8UC1);
                
    for (int i=sp.row_start; i<sp.row_end; i++){
        for (int j=sp.col_start; j<sp.col_end; j++)
        {
            if (sp.ILabel.at<int>(i,j) == sp.label)
                *(Msp.data\
                + Msp.step[0]*(i-sp.row_start)\
                + Msp.step[1]*(j-sp.col_start)\
                ) = 1;
            else
            {
                *(Msp.data\
                + Msp.step[0]*(i-sp.row_start)\
                + Msp.step[1]*(j-sp.col_start)\
                ) = 0;
            }
        }
    }
    return Msp;
}

SubMatLocation getBkPos(const SuperPixel sp, int ns){
    if (ns%2 == 0)
        ns--;
    int cr = (sp.row_start + sp.row_end)/2;
    int cc = (sp.col_start + sp.col_end)/2;
    int shift = (ns-1)/2;
    
    SubMatLocation ret;
    if (cc + shift >= sp.I.cols)
    {
        ret.col_start = sp.I.cols - 1 - shift*2;
        ret.col_end   = sp.I.cols - 1;
    }
    else if (cc - shift < 0)
    {
        ret.col_start = 0;
        ret.col_end   = shift*2;
    }
    else
    {
        ret.col_start = cc - shift;
        ret.col_end   = cc + shift;
    }

    if (cr + shift >= sp.I.rows)
    {
        ret.row_start = sp.I.rows - 1 - shift*2;
        ret.row_end   = sp.I.rows - 1;
    }
    else if (cr - shift < 0)
    {
        ret.row_start = 0;
        ret.row_end   = shift*2;
    }
    else
    {
        ret.row_start = cr - shift;
        ret.row_end   = cr + shift;
    }
    
    return ret;
}

SubMatLocation getRefPos(const cv::Mat Xk, const cv::Mat Bkt, const cv::Mat Msp){
    SubMatLocation ret;
    long long int minE = 0x7FFFFFFFFFFFFFFF;

    for (int u=0; u<Bkt.rows-Xk.rows; u++)
        for (int v=0; v<Bkt.cols-Xk.cols; v++)
        {
            cv::Mat Bkt_uv = Bkt.rowRange(u, u+Xk.rows).\
                                 colRange(v, v+Xk.cols);
            cv::Mat absAns, powAns;
            cv::absdiff(Bkt_uv, Xk, absAns);
            cv::pow(absAns, 2, powAns);
            cv::Scalar sumAns = cv::sum(powAns.mul(Msp));
            if (sumAns[0] < minE){
                minE = sumAns[0];
                ret.row_start = u;
                ret.col_start = v;
                ret.row_end   = u + Xk.rows;
                ret.col_end   = v + Xk.cols;
            }
        }
    return ret;
}

SpatialTemporalBuffer getM0(const cv::Mat Xk,\
                            const SpatialTemporalBuffer T0,\
                            const int epsilon)
{
    const int bufferSize = T0.size();
    SpatialTemporalBuffer M0(bufferSize);

    for (int i=-bufferSize+1; i<=0; i++){
        cv::Mat M0_i, M0temp;
        cv::absdiff(Xk, T0[i], M0temp);
        cv::threshold(M0temp, M0_i, epsilon, 1, cv::THRESH_BINARY);
        M0.update(M0_i);
    }
    return M0;
}

cv::Mat getMrainT(const SpatialTemporalBuffer M0,\
                  const int epsilon){
    const int bufferSize = M0.size();

    cv::Mat MrainT, SumMrainT = cv::Mat::zeros(M0[0].rows, M0[0].cols, M0[0].type());
    for (int i=-bufferSize+1; i<=0; i++)
        SumMrainT += M0[i];
    cv::threshold(SumMrainT, MrainT, epsilon, 1, cv::THRESH_BINARY); 
    return MrainT;
}

cv::Mat getObjectEdge(const cv::Mat IchannelU,\
              const cv::Mat IchannelV,\
              const int epsilon){
    /* Use Laplacian to get gradient currently */
    cv::Mat IchannelULap, IchannelVLap;
    cv::Laplacian(IchannelU, IchannelULap, -1, 5);
    cv::Laplacian(IchannelV, IchannelVLap, -1, 5);

    cv::Mat SumMat = IchannelULap + IchannelVLap, Me;
    cv::threshold(SumMat, Me, epsilon, 1, cv::THRESH_BINARY); 
    return Me;
}

cv::Mat getXavg(const SpatialTemporalBuffer Bk,\
                const cv::Mat Xk,\
                const cv::Mat Mrsp){
    const int bufferSize = Bk.size();
    cv::Mat Xavg = cv::Mat::zeros(Xk.rows, Xk.cols, CV_16UC1), Bki;
    for (int i = -bufferSize+1; i<=0; i++){
        SubMatLocation pos = getRefPos(Xk, Bk[i], Mrsp);
        Bk[i].rowRange(pos.row_start, pos.row_end).colRange(pos.col_start, pos.col_end).
              convertTo(Bki, Xavg.type());
        Xavg = Xavg + Bki;
    }
    cv::Mat Xavg_final;
    Xavg = Xavg/bufferSize;
    Xavg.convertTo(Xavg_final, Xk.type());
    return Xavg_final;
}