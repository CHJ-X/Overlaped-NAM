/***

encoder

input: a R Mat
ouput: a 64*64 picture

*/
#include "decoder.h"
using namespace cv;

bool checkClear(Mat &R, Point lt, Point rb);
void drawPic(Mat &pic, Point lt, Point rb);
bool findNextFlag(Mat &R, Point &cur);

// unfinished
Point findMatchPoint(Mat &R, Point &cur);

/*
    input: picture to ouput | left top Point | right bottom Point
*/



Mat decoder(Mat R){

    Mat pic(PIC_SIZE_X, PIC_SIZE_Y);
    Point flagPoint(PIC_SIZE_X-1, PIC_SIZE_Y-1);
    Point matchPoint;
    while(findNextFlag(R, flagPoint)){
        matchPoint = findMatchPoint(R, flagPoint);
        drawPic(pic, matchPoint, flagPoint);
    }
    return pic;
}


bool findNextFlag(Mat &R, Point &cur)
{
    while (cur.x >= 0 && cur.y >= 0)
    {
        // 图片中为1且为未访问则可以为起始点
        if(R.at(cur.y, cur.x) == H_MATRIX || R.at(cur.y, cur.x) == V_MATRIX)
        {
            return true;
        }
        // right to left
        cur.x = (cur.x - 1) % PIC_SIZE_Y;
        if (cur.x == 0)
        {
            // bottom to top
            cur.y = cur.y - 1;
            if (cur.y < 0)
            {
                std::cout << "END DECODING" << std::endl;
                return false;
            }
        }
        
    }
    std::cout << "END DECODING" << std::endl;
    return false;
}


Point findMatchPoint(Mat &R, Point &curr){
    // find nearest point whose flag == 1

    // traverse R
    Point matchPoint;
    for (;;)
    {
        // if find point whose flag == 1
        matchPoint.set(0, 0);
        if (checkClear(R, matchPoint, curr))
        {
            return matchPoint;
        }
        
    }
}

bool checkClear(Mat &R, Point lt, Point rb)
{
    if (lt == rb)
    {
        return true;
    }
    for (int i = lt.y; i <= rb.y; i++)
    {
        for (int j = lt.x; j <= rb.x; j++)
        {
            if (R.at(i, j))
            {
                return false;
            }
            
        }
    }
    return true;
}

void drawPic(Mat &pic, Point lt, Point rb)
{
    if (lt == rb)
    {
        pic.at(lt) = 1;
    }
    
    for (int i = lt.y; i <= rb.y; i++)
    {
        for (int j = lt.x; j <= rb.x; j++)
        {
            pic.at(i, j) = 1;
        }
    }

    return;
}
