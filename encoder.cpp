/***

encoder

input: a 64*64 picture
ouput: a R Mat

*/

#include<string>
#include"encoder.h"
using namespace cv;

std::string point2str(const Point& p);


/* 
input: a 64*64 picture | current search position
output: a init point whose flag == START
*/
//寻找初始点
bool findInit(Mat &pic, Mat &flag, Point &cur);
bool expandX(Mat& pic, Mat& flag, Point& lt, Point& rb);
bool expandY(Mat& pic, Mat& flag, Point& lt, Point& rb);


// 检查是否完全重叠
/*
input: H的右下点 rbH, V的右下点 rbV
output: 是否完全重叠
*/
bool isCrossover(Point& rbH, Point& rbV);
bool checkLaw2(Mat& R, Point& ltV, Point& rbV);
bool checkOverlap(Mat& R, std::unordered_map<std::string, Point> &hashmap, Mat& flag, Point& lt, Point& rb);

/**/



Mat encoder(Mat& pic)
{
    std::unordered_map<std::string, Point> hashmap;
    Mat R = Mat::zeros(PIC_SIZE_X, PIC_SIZE_Y, CV_8UC1);
    Mat flag = Mat::zeros(PIC_SIZE_X, PIC_SIZE_Y, CV_8UC1);

    Point initPoint(0, 0);
    // 1. 找一个起始坐标，找不到时即退出循环完成编码
    while (findInit(pic, flag, initPoint)){

#ifdef DEBUG
        std::cout << "R" << std::endl;
        std::cout << R << std::endl;
#endif
        Point targetPoint(initPoint);
        
        // 2. 横向扩展
        expandX(pic, flag, initPoint, targetPoint);
        

        // 孤立点检测
        if (targetPoint == initPoint)
        {
            R.at<uchar>(initPoint) = ISOLATED;
            continue;
        }
        

        // 3. 纵向扩展
        expandY(pic, flag, initPoint, targetPoint);

        // 4. 检查是否存在重叠情况
        checkOverlap(R, hashmap, flag, initPoint, targetPoint);

        // 5. 继续循环找起始点
    }

#ifdef DEBUG
    std::cout << "flag:" << std::endl;
    std::cout << flag << std::endl;
#endif
    
    return R;
}


std::string point2str(const Point& p)
{
     return std::to_string(p.x) + " " + std::to_string(p.y);
}

bool findInit(Mat& pic, Mat& flag, Point &cur)
{
    while (cur.x < PIC_SIZE_X && cur.y < PIC_SIZE_Y)
    {
        // 图片中为1且为未访问则可以为起始点
        if(pic.at<uchar>(cur.y, cur.x) == START && flag.at<uchar>(cur.y, cur.x) == UNVISITED)
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

bool expandX(Mat& pic, Mat& flag, Point& lt, Point& rb)
{
    do
    {
        rb.x++;
    } while (rb.x < PIC_SIZE_Y &&  pic.at<uchar>(rb) == 1 && flag.at<uchar>(rb) != OVERLAPPED);
    rb.x--;
    if (lt.x == rb.x)
    {
        return false;
    }
    
    return true;
}

bool expandY(Mat& pic, Mat& flag, Point& lt, Point& rb)
{
    bool isBlock = true;
    do
    {
        rb.y++;
        if (rb.y >= PIC_SIZE_Y)
        {
            break;
        }
        for (int i = lt.x; i <= rb.x; i++)
        {
            
            if (pic.at<uchar>(rb.y, i) == 1 && flag.at<uchar>(rb.y, i) != OVERLAPPED)
            {
                //std::cout << "-----------------\n";
                //std::cout << "pic" << pic.at<uchar>(rb.y, i) << std::endl;;
                //std::cout << "-----------------\n";
                continue;
            }
            else
            {

                ////std::cout << "bug!!!!!!" << std::endl;
                ////std::cout << pic.at<uchar>(rb) << std::endl;
                isBlock = false;
                break;
            }
            
        }
        
    } while (isBlock);
    rb.y--;
    if (rb.y == lt.y)
    {
        return false;
    }
    return true;
}

bool isCrossover(Point& rbH, Point& rbV)
{
    if (rbH.x > rbV.x && rbH.y < rbV.y)
    {
        return true;
    }
    return false;
}

bool checkLaw2(Mat& R, Point& ltV, Point& rbV)
{
    if (ltV.y == 0)
    {
#ifdef DEBUG
        std::cout << "checkLaw2: true" << std::endl;
#endif // DEBUG

        
        return true;
    }
    
    for (int i = 0; i < ltV.y; i++)
    {
        for (int j = ltV.x; j <= rbV.x; j++)
        {
            if (R.at<uchar>(i, j) == START)
            {
#ifdef DEBUG
                std::cout << "checkLaw2: false" << std::endl;
#endif // DEBUG
                return false;
            }
            
        }
    }
#ifdef DEBUG
    std::cout << "checkLaw2: true" << std::endl;
#endif // DEBUG

    return true;
}
Point findLtV(Mat& R, Point p) {
#ifdef DEBUG
    std::cout << "p: " << p.x << " " << p.y << std::endl;
#endif // DEBUG


    
    for (int i = p.y; i >= 0; i--)
    {
#ifdef DEBUG
        std::cout << "R in this:" << std::endl;
        std::cout << R << std::endl;
        std::cout << "i: " << i << std::endl;
        std::cout << R.at<uchar>(i, p.x) << std::endl;
#endif // DEBUG

        
        if (R.at<uchar>(i, p.x) == START)
        {
#ifdef DEBUG
            std::cout << "ltV will return as: " << p.x << " " << i << std::endl;
#endif // DEBUG
            return Point(p.x, i);
        }
    }
    return Point(-1, -1);

};
bool checkOverlap(Mat& R, std::unordered_map<std::string, Point> &hashmap, Mat& flag, Point& lt, Point& rb)
{
#ifdef DEBUG
    std::cout << "lt: " << lt.x << " " << lt.y << std::endl;
    std::cout << "rb: " << rb.x << " " << rb.y << std::endl;
#endif // DEBUG



    if (lt.y == 0)
    {
        // 不重叠
        R.at<uchar>(lt) = START;
        R.at<uchar>(rb) = H_MATRIX;
        hashmap[point2str(lt)] = rb;
        setRectInMat(flag, lt, rb, VISITED);
#ifdef DEBUG
        std::cout << "overlapped: false" << std::endl;
        std::cout << "position1: lt.y == 0" << std::endl;
#endif // DEBUG

        return false;
    }
    bool hasOverlapped = false;

    

    for (int i = lt.x; i <= rb.x; i++)
    {
        if (flag.at<uchar>(lt.y - 1, i) == VISITED)
        {
            Point ltV = findLtV(R, Point(i, lt.y - 1));
            Point rbV = hashmap[point2str(ltV)];
            i = rbV.x + 1;
#ifdef DEBUG
            std::cout << "ltV: " << ltV.x << " " << ltV.y << std::endl;
            std::cout << "rbV: " << rbV.x << " " << rbV.y << std::endl;
            std::cout << "position: hasOverlapped == false" << std::endl;
#endif // DEBUG
            if (isCrossover(rb, rbV))
            { 
#ifdef DEBUG
                std::cout << "isCrossover" << std::endl;
#endif // DEBUG

                
                if(checkLaw2(R, ltV, rbV) && !hasOverlapped)
                {
                    // 完全重叠且可以重叠，不用分割
#ifdef DEBUG
                    std::cout << "position: hasOverlapped == false" << std::endl;
#endif // DEBUG
                    hasOverlapped = true;
                    setRectInMat(flag, lt, rb, OVERLAPPED);
                    setRectInMat(flag, ltV, rbV, OVERLAPPED);
                    R.at<uchar>(rbV) = V_MATRIX;
                }
                else
                {
                    // 完全重叠但是不可重叠，需要分割两部分
                    Point newRb(lt.y - 1, rbV.x);
                    Point newLt(rb.y + 1, ltV.x);
                    R.at<uchar>(newRb) = H_MATRIX;
                    R.at<uchar>(newLt) = START;
                    hashmap[point2str(ltV)] = newRb;
                    hashmap[point2str(newLt)] = rbV;
                }
                
            }
            else if (rbV.y > rb.y)
            {
#ifdef DEBUG
                std::cout << "is not Crossover" << std::endl;
                std::cout << "case 1" << std::endl;
#endif // DEBUG

                // 部分重叠但是上下两段，需要分割两部分
                Point newRb(lt.y - 1, rbV.x);
                Point newLt(rb.y + 1, ltV.x);
                R.at<uchar>(newRb) = H_MATRIX;
                R.at<uchar>(newLt) = START;
                hashmap[point2str(ltV)] = newRb;
                hashmap[point2str(newLt)] = rbV;
            }
            else
            {
#ifdef DEBUG
                std::cout << "is not Crossover" << std::endl;
                std::cout << "case 1" << std::endl;
#endif // DEBUG
                // 部分重叠，修改R矩阵进行分割
                R.at<uchar>(rbV) = 0;
                rbV = Point(lt.y - 1, rbV.x);
                R.at<uchar>(rbV) = H_MATRIX;
                hashmap[point2str(ltV)] = rbV;
            }

        }
        
    }
    R.at<uchar>(lt) = START;
    R.at<uchar>(rb) = H_MATRIX;
    hashmap[point2str(lt)] = rb;
    if (!hasOverlapped)
    {
        setRectInMat(flag, lt, rb, VISITED);
#ifdef DEBUG
        std::cout << "overlapped: false" << std::endl;
        std::cout << "position1: hasOverlapped == false" << std::endl;
#endif // DEBUG

        
        return false;
    }
#ifdef DEBUG
    std::cout << "overlapped: true" << std::endl;
#endif // DEBUG
    return true;
}


