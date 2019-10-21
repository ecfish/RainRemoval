cmake_minimum_required(VERSION 3.10)

set(CMAKE_BUILD_TYPE        "Debug")
set(CMAKE_CXX_FLAGS_DEBUG   "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

project(RainRemoval)

set(OpenCV_DIR  "/usr/local/OpenCV4") # OpenCV 4.x
#set(OpenCV_DIR  "/usr/share/OpenCV") # OpenCV 3.x
find_package(OpenCV REQUIRED)

message(STATUS "version: ${OpenCV_VERSION}")

include_directories(include)
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(lib)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} RAIN_REMOVAL_LIB)
