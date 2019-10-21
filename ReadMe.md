# 基于超像素的视频去雨简化实现
## 概述
 去雨算法基于论文*Chen J , Tan C H , Hou J , et al. Robust Video Content Alignment and Compensation for Rain Removal in a CNN Framework[J]. 2018*， 并使用C++代码实现。代码主要复现了论文3.1节内容，并未完全复现论文算法，主要有以下不同：
 1. 原论文3.2节中CNN网络用于补偿超像素平均模糊的结果，对于去除雨滴效果不大，暂未实现；
 2. 原文中Xk为包含对应超像素的正方形区域，代码实现为包含对应超像素的最小矩形区域，从结果看，去雨效果依然良好；
 3. 论文中Bk张量包含当前帧的前后相邻帧，考虑到系统的因果性，代码仅取当前帧的相邻前几帧
 4. 原文中T1为误差E最小的nst个Bk子分量，代码在这里做了简化处理，直接取Bk张量中的所有子分量作平均，当视频中物体无快速移动的时候，差异较小

## 环境
1. OpenCV version 4.1.1
2. g++ version 7.4.0
3. ubuntu 18.04.3 LTS
## 编译
```Shell
mkdir build
cd build
cmake ..
make all
```
## 运行
```Shell
./RainRemoval ${video_path}
```
这里在data文件夹下提供两段带雨的测试视频
```Shell
# 进入build的目录后
./RainRemoval ../data/test1.mp4 #第一段视频
./RainRemoval ../data/test2.mp4 #第二段视频
```
运行成功后命令行会提示当前处理的视频帧，其中默认参数设置前10帧不作任何处理，从第11帧开始结合前10帧内容作模糊去雨处理