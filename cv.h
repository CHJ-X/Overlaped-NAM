#pragma once
#include<vector>

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
        Mat(Mat &);
        ~Mat();
        void clear();

        int &at(int y, int x);
        int &at(const Point &p);
    };
}