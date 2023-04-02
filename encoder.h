#pragma once
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<unordered_map>

#define DEBUG


#define PIC_SIZE_X 256
#define PIC_SIZE_Y 256
#define WHITE 255


enum NODE_TYPE
{
    ISOLATED = 4,
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

inline void setRectInMat(cv::Mat& m, cv::Point lt, cv::Point rb, int value)
{
    rb.x++;
    rb.y++;
    cv::Mat Roi = m(cv::Rect(lt, rb));
    Roi = value;
}

cv::Mat encoder(cv::Mat& pic);
