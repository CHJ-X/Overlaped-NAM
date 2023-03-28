#pragma once
#include<vector>
#include<iostream>
namespace cv{
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

        bool operator==(const Point& other)
        {
            if (other.x == this->x && other.y == this->y)
            {
                return true;
            }
            return false;
        }
        void set(int xx, int yy){
            this->x = xx;
            this->y = yy;
        }
    };

    class Mat
    {
    public:
        std::vector<std::vector<int>> elem;
        Mat(int m, int n);
        // Mat(Mat &);
        //~Mat();
        friend std::ostream &operator<<(std::ostream& os, const Mat& m){
            for (auto its : m.elem)
            {
                for (auto it : its)
                {
                    os << it << " ";
                }
                os << std::endl;
            }
            return os;
        }
        void clear();

        int &at(int y, int x);
        int &at(const Point &p);

        void set(const Point &lt, const Point &rb, int value);
        // int sum(const Point &lt, const Point &rb);
    };
}