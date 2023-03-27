/***

encoder

input: a 64*64 picture
ouput: a R Mat

*/

#include"encoder.h"
using namespace cv;

//寻找初始点
bool findInit(Mat &pic, Mat &flag, Point &cur);

/*
只有声明，还未实现的函数
*/

/* 
input: a 64*64 picture | current search position
output: a init point whose flag == 1
*/


int expandX(Mat& pic, Mat& flag, Point& lt, Point& rb);

int expandY(Mat& pic, Mat& flag, Point& lt, Point& rb);

void checkOverlap(Mat& pic, Mat& flag, Point& lt, Point& rb);

void checkLaw2(Mat& pic, Mat& flag, Point& lt, Point& rb);

void checkLaw3(Mat& pic, Mat& flag, Point& lt, Point& rb);

/**/

Mat encoder(Mat& pic)
{

    Mat R(PIC_SIZE_X, PIC_SIZE_Y);
    Mat flag(PIC_SIZE_X, PIC_SIZE_Y);
    R.clear();
    flag.clear();

    Point initPoint(0, 0);
    // 1. 找一个起始坐标，找不到时即退出循环完成编码
    while (findInit(pic, flag, initPoint)){
        Point targetPoint;
        // 2. 横向扩展
        targetPoint.x = expandX(pic, flag, initPoint, targetPoint);

        // 3. 纵向扩展
        targetPoint.y = expandY(pic, flag, initPoint, targetPoint);

        // 4. 检查是否存在重叠情况
        checkOverlap(pic, flag, initPoint, targetPoint);
        checkLaw2(pic, flag, initPoint, targetPoint);
        checkLaw3(pic, flag, initPoint, targetPoint);

        // 5. 继续循环找起始点
    }

    return R;
}


bool findInit(Mat& pic, Mat& flag, Point &cur)
{
    while (cur.x < PIC_SIZE_X && cur.y < PIC_SIZE_Y)
    {
        // 图片中为1且为未访问则可以为起始点
        if(pic.at(cur.y, cur.x) == 1 && flag.at(cur.y, cur.x) == 0)
        {
            return true;
        }
        // left to right
        cur.x = (cur.x + 1) % PIC_SIZE_Y;
        if (cur.x == 0)
        {
            // top to bottom
            cur.y = cur.y + 1;
            if (cur.y >= PIC_SIZE_Y)
            {
                std::cout << "END ENCODING" << std::endl;
                return false;
            }
        }
        
    }
    std::cout << "END ENCODING" << std::endl;
    return false;
}