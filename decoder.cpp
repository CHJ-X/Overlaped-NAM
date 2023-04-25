/***

encoder

input: a R Mat
ouput: a 64*64 picture

*/
#include"Codec.h"
using namespace cv;

Mat Codec::decode(Mat& R)
{

    Mat newPic = Mat::zeros(this->rows, this->cols, CV_8UC1);
    //使用一个x越界的点作为起始点
    Point flagPoint(cols, rows - 1);
    Point matchPoint;

    while (findNextFlag(R, flagPoint)) {
        matchPoint = findMatchPoint(R, flagPoint);
        drawPic(newPic, matchPoint, flagPoint);
        R.at<uchar>(matchPoint) = 0;
    }
    std::cout << "END DECODING" << std::endl;
    return newPic;
}





bool Codec::findNextFlag(Mat &R, Point &cur)
{
    while (cur.x >= 0 && cur.y >= 0)
    {
        
        // right to left
        cur.x = (cur.x - 1 + cols) % cols;
        if (cur.x == 0)
        {
            // bottom to top
            cur.y = cur.y - 1;
            if (cur.y < 0)
            {
                return false;
            }
        }
        // 图片中为1且为未访问则可以为起始点
        if (R.at<uchar>(cur.y, cur.x) == NODE_TYPE::H_MATRIX || 
            R.at<uchar>(cur.y, cur.x) == NODE_TYPE::V_MATRIX ||
            R.at<uchar>(cur.y, cur.x) == NODE_TYPE::ISOLATED)
        {
            return true;
        }
    }
    return false;
}

//
Point Codec::findMatchPoint(Mat &R, Point &cur){
    if (R.at<uchar>(cur) == NODE_TYPE::ISOLATED)
    {
        return cur;
    }
    // find nearest point whose flag == START
    if (R.at<uchar>(cur) == NODE_TYPE::H_MATRIX)
    {
        for (int i = cur.y; i >= 0; i--)
        {
            for (int j = cur.x; j >= 0; j--)
            {
                if (R.at<uchar>(i, j) == NODE_TYPE::START && checkClear(R, Point(j, i), cur))
                {
                    return Point(j, i);
                }
                
            }
        }
    }
    else if(R.at<uchar>(cur) == NODE_TYPE::V_MATRIX)
    {
        for (int i = cur.x; i >= 0; i--)
        {
            for (int j = cur.y; j >= 0; j--)
            {
                if (R.at<uchar>(j, i) == NODE_TYPE::START && checkClear(R, Point(i, j), cur))
                {
                    return Point(i, j);
                }
                
            }
        }
    }
    std::cout << "ERROR: Can not find match point" << std::endl;
    return Point(-1, -1);
}

bool Codec::checkClear(Mat &R, Point lt, Point rb)
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
            if (R.at<uchar>(i, j))
            {
                return false;
            }
        }
    }
    return true;
}

void Codec::drawPic(Mat &pic, Point lt, Point rb)
{
    rb.x++;
    rb.y++;
    Mat roi = pic(Rect(lt, rb));
    roi.setTo(WHITE);
    return;
}
