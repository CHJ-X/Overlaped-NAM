#include"cv.h"

using namespace cv;
Mat::Mat(int m, int n)
{
    elem.resize(m);
    for (int i = 0; i < m; i++)
    {
        elem[i].resize(n);
    }
}

// Mat::Mat(Mat & newMat)
// {
    
// }

// Mat::~Mat()
// {

// }

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

void Mat::set(const Point &lt, const Point &rb, int value)
{
    for (int i = lt.y; i <= rb.y; i++)
    {
        for (int j = lt.x; j <= rb.x; j++)
        {
            /*std::cout << "i:" << i << std::endl;
            std::cout << "j:" << j << std::endl;
            std::cout << "elem.size(): " << elem.size() << std::endl;
            std::cout << "elem[0].size(): " << elem[0].size() << std::endl;
            std::cout << "elem[i][j]: " << elem[i][j] << std::endl;*/
            elem[i][j] = value;
            //std:: cout << "here";
        }
    }
    
}

//int Mat::sum(const Point &lt, const Point &rb)
//{
//    int sum = 0;
//    for (int i = lt.y; i <= rb.y; i++)
//    {
//        for (int j = lt.x; j <= rb.x; j++)
//        {
//            sum += this->elem[i][j];
//        }
//    }
//    return sum;
//}
