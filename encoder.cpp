/***

encoder

input: a 64*64 picture
ouput: a R Mat

*/

#include"encoder.h"
using namespace cv;

/* 
input: a 64*64 picture | current search position
output: a init point whose flag == 1
*/
Point findInit(Mat& pic, Mat& flag, Point &cur)
{
    // left to right
    // top to bottom
}

int expandX(Mat& pic, Mat& flag, Point& lt, Point& rb);

int expandY(Mat& pic, Mat& flag, Point& lt, Point& rb);

void checkOverlap(Mat& pic, Mat& flag, Point& lt, Point& rb);

void checkLaw2(Mat& pic, Mat& flag, Point& lt, Point& rb);

void checkLaw3(Mat& pic, Mat& flag, Point& lt, Point& rb);

Mat encoder(Mat& pic)
{

    Mat R(PIC_SIZE_X, PIC_SIZE_Y);
    Mat flag(PIC_SIZE_X, PIC_SIZE_Y);
    R.clear();
    flag.clear();

    Point initPoint(0, 0);
    initPoint = findInit(pic, flag, initPoint);
    // 1. 找一个起始坐标，找不到时即退出循环完成编码
    while (initPoint.x != PIC_SIZE_X - 1 && initPoint.y != PIC_SIZE_Y - 1){
        initPoint = findInit(pic, flag, initPoint);
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
        initPoint = findInit(pic, flag, initPoint);
    }

    return R;
}