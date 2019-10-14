/*
 * Created by zhanghan, 2019.10.13
 */

#include <stBuffer.h>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <Exceptions.h>

SpatialTemporalBuffer::SpatialTemporalBuffer (const int buffer_size){
    _buffer = (cv::Mat **)malloc(sizeof(cv::Mat *)*buffer_size);
    _buffer_size  = buffer_size;
    
    _qHead        = &_buffer[0];
    _buffer_start = &_buffer[0];
    _buffer_end   = &_buffer[_buffer_size-1];

    itemNum = 0;
}

SpatialTemporalBuffer::~SpatialTemporalBuffer(){
    for (cv::Mat** iter = _buffer_start; iter <= _buffer_end; iter++)
        free(*iter);
    free(_buffer);
}

cv::Mat SpatialTemporalBuffer::operator[] (const int index){
    if (index <= -_buffer_size || index > 0 || index < -(itemNum-1))
        throw STBEXCEPTION::ST_BUFFER_INDEX_OUT_OF_RANGE;

    if (_qHead + index - 1 < _buffer_start)
        return **(_qHead + index - 1 + _buffer_size);
    else
        return **(_qHead + index - 1);
}

void SpatialTemporalBuffer::update(cv::Mat *const n_input){
    if (itemNum >= _buffer_size)
        free(*_qHead);
    *_qHead = n_input;
    _qHead  = _qHead + 1 > _buffer_end ? \
              _qHead + 1 - _buffer_size : _qHead + 1;
    itemNum = itemNum + 1 > _buffer_size ? \
              _buffer_size : itemNum + 1;
}

SpatialTemporalBuffer SpatialTemporalBuffer::spatialAt(const int sx,\
                                                       const int sy,\
                                                       const int ex,\
                                                       const int ey){
    SpatialTemporalBuffer newstBuffer(_buffer_size);
    cv::Mat *bufferi;
    for (int i = -itemNum + 1; i<=0; i++)
    {
        bufferi = new cv::Mat();
        *bufferi = ((*this)[i]).rowRange(sx, ex).colRange(sy, ey);
        newstBuffer.update(bufferi);
    }
    return newstBuffer;
}

