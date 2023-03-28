#pragma once
#include"cv.h"
#include<iostream>
#include<unordered_map>
#define PIC_SIZE_X 8
#define PIC_SIZE_Y 8

enum NODE_TYPE
{
    ISOLATED = -1,
    START = 1,
    H_MATRIX = 2,
    V_MATRIX = 3
};

enum FLAG_TYPE
{
    UNVISITED = 0,
    VISITED = 1,
    OVERLAPPED = 2
};


cv::Mat encoder(cv::Mat& pic);