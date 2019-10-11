cmake_minimum_required(VERSION 3.10)

project(RainRemoval)

find_package(OpenCV REQUIRED)

include_directories(include)
add_subdirectory(lib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} RAIN_REMOVAL_LIB)