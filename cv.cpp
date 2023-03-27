#include"cv.h"

using namespace cv;
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

int &Mat::at(int y, int x)
{
    return elem[y][x];
}

int &Mat::at(const cv::Point &p)
{
    return elem[p.y][p.x];
}