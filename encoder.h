#pragma once
#include<iostream>
#include<vector>
#define PIC_SIZE_X 64
#define PIC_SIZE_Y 64

namespace cv{
    extern int aaa = 100;
    class Mat
    {
    public:
        std::vector<std::vector<int>>* ptr;
        Mat(int m, int n);
        Mat(Mat &);
        ~Mat();
        void clear();
    };

    Mat::Mat(int m, int n)
    {
        ptr = new std::vector<std::vector<int>>(m, std::vector<int>(n));
    }

    Mat::Mat(Mat & newMat)
    {
        
    }

    Mat::~Mat()
    {
    }

    void Mat::clear()
    {

    }

    class Point
    {
    public:
        int x, y;
        Point(int xx, int yy){
            x = xx;
            y = yy;
        }
        Point(){
            x = 0;
            y = 0;
        }
        void set(int xx, int yy){
            this->x = xx;
            this->y = yy;
        }
    };
}



