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
        std::vector<std::vector<int>> elem;
        Mat(int m, int n);
        Mat(Mat &);
        ~Mat();
        void clear();

        int at(int y, int x);
    };

    Mat::Mat(int m, int n)
    {
        elem.resize(m);
        for (auto it : elem)
        {
            it.resize(n);
        }
        
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

    int Mat::at(int y, int x)
    {
        return elem[y][x];
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



