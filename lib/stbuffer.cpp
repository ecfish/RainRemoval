/*
 * Created by zhanghan, 2019.10.13
 */

#include <stbuffer.h>
#include <opencv2/opencv.hpp>
#include <cstring>
#include <Exceptions.h>

SpatialTemporalBuffer::SpatialTemporalBuffer (const int buffer_size){
    _buffer       = new cv::Mat[buffer_size];
    _buffer_size  = buffer_size;
    
    _qHead        = _buffer;
    _itemNum      = 0;
}

SpatialTemporalBuffer::~SpatialTemporalBuffer (){
}

cv::Mat SpatialTemporalBuffer::operator[] (const int index) const{
    if (index <= -_buffer_size || index > 0 || index < -(_itemNum-1))
        throw STBEXCEPTION::ST_BUFFER_INDEX_OUT_OF_RANGE;

    if (_qHead + index - 1 < _buffer)
        return *(_qHead + index - 1 + _buffer_size);
    else
        return *(_qHead + index - 1);
}

void SpatialTemporalBuffer::update(const cv::Mat matItem){
    *_qHead  = matItem;
    _qHead   = _qHead + 1 >= _buffer + _buffer_size ? \
               _qHead + 1 - _buffer_size : _qHead + 1;
    _itemNum = _itemNum + 1 > _buffer_size ? \
               _buffer_size : _itemNum + 1;
}

SpatialTemporalBuffer SpatialTemporalBuffer::spatialAt(const int sx,\
                                                       const int sy,\
                                                       const int ex,\
                                                       const int ey,\
                                                       const int channel) const{
    SpatialTemporalBuffer newstBuffer(_buffer_size);
    for (int i = -_itemNum + 1; i<=0; i++){
        cv::Mat newMat = ((*this)[i]).rowRange(sx, ex).colRange(sy, ey);
        if (channel == -1)
            newstBuffer.update(newMat);
        else{
            cv::Mat newMatChannel(newMat.rows, newMat.cols, CV_8UC1);
            cv::mixChannels(newMat, newMatChannel, new int[2]{channel, 0}, 1);
            newstBuffer.update(newMatChannel);
        }
    }
    return newstBuffer;
}

bool SpatialTemporalBuffer::isFull() const{
    return (_itemNum >= _buffer_size);
}

int SpatialTemporalBuffer::size() const{
    return _buffer_size;
}