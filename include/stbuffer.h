/*
 * Created by zhanghan, 2019.10.18
 */

#ifndef __STBUFFER_H__
#define __STBUFFER_H__

#include <configs.h>
#include <opencv2/opencv.hpp>
#include <memory>

class SpatialTemporalBuffer{
public:
    /* Initialized as a buffer in size of buffer_size*/
     SpatialTemporalBuffer(const int buffer_size);

    /* release */
    ~SpatialTemporalBuffer();

    /* Get the index(th) latest mat item in buffer, 
     * index should be in [-_itemNum + 1, 0]. 
     */
    cv::Mat operator[](const int index) const;
    
    /* Input a new mat item to buffer. The earliest item will be replaced
     * if the buffer is full.
     */
    void update(const cv::Mat matItem);

    /* Get the submat of each mat item in buffer,
     * the row range is [sx, ex] and the col range is [sy, ey] 
     */
    SpatialTemporalBuffer spatialAt(const int sx,\
                                    const int sy,\
                                    const int ex,\
                                    const int ey,\
                                    const int channel) const;

    /* Whether the buffer is full. */
    bool isFull() const;

    /* Get buffer size */
    int size() const;
    
private:
    cv::Mat *_buffer;
    cv::Mat *_qHead;
    int _itemNum;
    int _buffer_size;
};

#endif