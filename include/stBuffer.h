/*
 * Created by zhanghan, 2019.10.13
 */
#include <configs.h>
#include <opencv2/opencv.hpp>

class SpatialTemporalBuffer{
public:
    SpatialTemporalBuffer(const int buffer_size = NT);
    ~SpatialTemporalBuffer();

    cv::Mat operator[](const int index);
    void update(cv::Mat *const n_input);

    SpatialTemporalBuffer spatialAt(const int sx,\
                                    const int sy,\
                                    const int ex,\
                                    const int ey);
    
    int itemNum;
    cv::Mat **_buffer;
    /* Not implemented yet */
private:
    
    int _buffer_size;
    cv::Mat **_qHead;

    cv::Mat **_buffer_start;
    cv::Mat **_buffer_end;
};