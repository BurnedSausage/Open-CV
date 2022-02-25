cmake_minimum_required(VERSION 3.10)
project(opencv-project)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wconversion")

find_package(OpenCV 4.2.0 REQUIRED)
add_executable(opencv-project main.cpp Regocnizer.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv-project ${OpenCV_LIBRARIES})