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
    Mat R(PIC_SIZE_X, PIC_SIZE_Y);
    Mat flag(PIC_SIZE_X, PIC_SIZE_Y);
    R.clear();
    flag.clear();

    Point initPoint(0, 0);
    // 1. 找一个起始坐标，找不到时即退出循环完成编码
    while (findInit(pic, flag, initPoint)){
        Point targetPoint(initPoint);
        
        // 2. 横向扩展
        expandX(pic, flag, initPoint, targetPoint);
        

        // 孤立点检测
        if (targetPoint == initPoint)
        {
            R.at(initPoint) = ISOLATED;
            continue;
        }
        

        // 3. 纵向扩展
        expandY(pic, flag, initPoint, targetPoint);

        // 4. 检查是否存在重叠情况
        checkOverlap(R, hashmap, flag, initPoint, targetPoint);

        // 5. 继续循环找起始点
    }

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
        if(pic.at(cur.y, cur.x) == START && flag.at(cur.y, cur.x) == UNVISITED)
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
    } while (rb.x < PIC_SIZE_Y &&  pic.at(rb) == 1 && flag.at(rb) != OVERLAPPED);
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
        std::cout << "RB==y" << rb.y << std::endl;
        if (rb.y >= PIC_SIZE_Y)
        {
            break;
        }
        for (int i = lt.x; i <= rb.x; i++)
        {
            if (pic.at(i, rb.y) == 1 && flag.at(i, rb.y) != OVERLAPPED)
            {
                //std::cout << "-----------------\n";
                //std::cout << "pic" << pic.at(rb.y, i) << std::endl;;
                //std::cout << "-----------------\n";
                continue;
            }
            else
            {

                ////std::cout << "bug!!!!!!" << std::endl;
                ////std::cout << pic.at(rb) << std::endl;
                isBlock = false;
                break;
            }
            
        }
        
    } while (isBlock);
    std::cout << "rby" << rb.y << std::endl;
    rb.y--;

    std::cout << "here" << std::endl;
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
    if (ltV.x == 0)
    {
        return true;
    }
    
    for (int i = 0; i < ltV.y; i++)
    {
        for (int j = ltV.x; j <= rbV.x; j++)
        {
            if (R.at(i, j) == START)
            {
                return false;
            }
            
        }
    }
    return true;
}

bool checkOverlap(Mat& R, std::unordered_map<std::string, Point> &hashmap, Mat& flag, Point& lt, Point& rb)
{
    if (lt.y == 0)
    {
        // 不重叠
        R.at(lt) = START;
        R.at(rb) = H_MATRIX;
        hashmap[point2str(lt)] = rb;
        flag.set(lt, rb, VISITED);
        return false;
    }
    bool hasOverlapped = false;

    auto findLtV = [&R] (Point p){
        for (int i = 0; i < p.y; i++)
        {
            if (R.at(i, p.x) == START)
            {
                return Point(i, p.x);
            }
        }
        
    };

    for (int i = lt.x; i <= lt.y; i++)
    {
        if (flag.at(lt.y - 1, i) == VISITED)
        {
            Point ltV = findLtV(Point(lt.y - 1, i));
            Point rbV = hashmap[point2str(ltV)];
            if (isCrossover(rb, rbV))
            { 
                if(checkLaw2(R, ltV, rbV) && !hasOverlapped)
                {
                    // 完全重叠且可以重叠，不用分割
                    hasOverlapped = true;
                    flag.set(lt, rb, OVERLAPPED);
                    flag.set(ltV, rbV, OVERLAPPED);
                    R.at(rbV) = V_MATRIX;
                }
                else
                {
                    // 完全重叠但是不可重叠，需要分割两部分
                    Point newRb(lt.y - 1, rbV.x);
                    Point newLt(rb.y + 1, ltV.x);
                    R.at(newRb) = H_MATRIX;
                    R.at(newLt) = START;
                    hashmap[point2str(ltV)] = newRb;
                    hashmap[point2str(newLt)] = rbV;
                }
                
            }
            else if (rbV.y > rb.y)
            {
                // 部分重叠但是上下两段，需要分割两部分
                Point newRb(lt.y - 1, rbV.x);
                Point newLt(rb.y + 1, ltV.x);
                R.at(newRb) = H_MATRIX;
                R.at(newLt) = START;
                hashmap[point2str(ltV)] = newRb;
                hashmap[point2str(newLt)] = rbV;
            }
            else
            {
                // 部分重叠，修改R矩阵进行分割
                R.at(rbV) = 0;
                rbV = Point(lt.y - 1, rbV.x);
                R.at(rbV) = H_MATRIX;
                hashmap[point2str(ltV)] = rbV;
            }
        }
        
    }
    
    if (!hasOverlapped)
    {
        R.at(lt) = START;
        R.at(rb) = H_MATRIX;
        hashmap[point2str(lt)] = rb;
        std::cout << "R" << R << std::endl;
        std::cout << rb.x << " " << rb.y<< std::endl;
        std::cout << lt.x << " " << lt.y << std::endl;
        flag.set(lt, rb, VISITED);

        return false;
    }
}