/***

encoder

input: a R Mat
ouput: a 64*64 picture

*/
#include "decoder.h"
using namespace cv;

bool checkClear(Mat &R, Point lt, Point rb);


Point findNextFlag(Mat &R, Point &curr){
    //right to left
    //bottom to top
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

/*
    input: picture to ouput | left top Point | right bottom Point
*/

void drawPic(Mat &pic, Point lt, Point rb);

Mat decoder(Mat R){

    Mat pic(PIC_SIZE_X, PIC_SIZE_Y);
    Point flagPoint(PIC_SIZE_X-1, PIC_SIZE_Y-1);
    Point matchPoint;
    while(flagPoint.x != 0 && flagPoint.y != 0){
        flagPoint = findNextFlag(R, flagPoint);
        matchPoint = findMatchPoint(R, flagPoint);
        drawPic(pic, matchPoint, flagPoint);
    }
    return pic;
}