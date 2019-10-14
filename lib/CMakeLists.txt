add_library(RAIN_REMOVAL_LIB 
    SPAlgorithm.cpp
    VideoProcess.cpp
    SPProcess.cpp
    stBuffer.cpp
)
target_link_libraries(RAIN_REMOVAL_LIB ${OpenCV_LIBS})
