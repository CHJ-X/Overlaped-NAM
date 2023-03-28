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
Point findMatchPoint(Mat &R, Point &cur);

Mat decoder(Mat R){

    Mat pic(PIC_SIZE_X, PIC_SIZE_Y);
    Point flagPoint(PIC_SIZE_X-1, PIC_SIZE_Y-1);
    Point matchPoint;

    // 这里有点小问题
    while(findNextFlag(R, flagPoint)){
        matchPoint = findMatchPoint(R, flagPoint);
        drawPic(pic, matchPoint, flagPoint);
        std::cout << "pic in decoder" << std::endl;
        std::cout << pic << std::endl;
        R.at(matchPoint) = 0;
        std::cout << "R in decoder" << std::endl;
        std::cout << R << std::endl;
    }
    return pic;
}


bool findNextFlag(Mat &R, Point &cur)
{
    while (cur.x >= 0 && cur.y >= 0)
    {
        
        // right to left
        cur.x = (cur.x - 1 + PIC_SIZE_X) % PIC_SIZE_X;
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
        // 图片中为1且为未访问则可以为起始点
        if (R.at(cur.y, cur.x) == H_MATRIX || R.at(cur.y, cur.x) == V_MATRIX)
        {
            return true;
        }
    }
    std::cout << "END DECODING" << std::endl;
    return false;
}

// unfinished
Point findMatchPoint(Mat &R, Point &cur){
    // find nearest point whose flag == START
    if (R.at(cur) == H_MATRIX)
    {
        for (int i = cur.y; i >= 0; i--)
        {
            for (int j = cur.x; j >= 0; j--)
            {
                if (R.at(i, j) == START && checkClear(R, Point(j, i), cur))
                {
                    return Point(j, i);
                }
                
            }
        }
    }
    else if(R.at(cur) == V_MATRIX)
    {
        for (int i = cur.x; i >= 0; i--)
        {
            for (int j = cur.y; j >= 0; j--)
            {
                if (R.at(j, i) == START && checkClear(R, Point(i, j), cur))
                {
                    return Point(i, j);
                }
                
            }
        }
    }
    std::cout << "ERROR: Can not find match point" << std::endl;
    return Point(-1, -1);
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
            if (i == lt.y && j == lt.x || i == rb.y && j == rb.x)
            {
                continue;
            }
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

    pic.set(lt, rb, 1);
    return;
}
