add_library(RAIN_REMOVAL_LIB 
    spalgorithm.cpp
    videoprocess.cpp
    spprocess.cpp
    stbuffer.cpp
    colorspace.cpp
)
target_link_libraries(RAIN_REMOVAL_LIB ${OpenCV_LIBS})
