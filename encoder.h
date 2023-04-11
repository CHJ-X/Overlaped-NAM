#pragma once
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

#define DEBUG
#ifdef DEBUG
#define DEBUG_COUT std::cout << "DEBUG:"
#else
#define DEBUG_COUT 0 && std::cout
#endif // DEBUG

#ifdef DEBUG
#define DEBUG_COUT_INFO std::cout << "file: " << __FILE__ << "\n" \
 << "in function: " << __func__ << "\n"\
 << "rows: " << __LINE__ << std::endl
#else
#define DEBUG_COUT_INFO 0 && std::cout
#endif // DEBUG


#define WHITE 255

using cv::Mat;
using cv::Point;
using cv::Rect;
using std::string;
using std::vector;


enum NODE_TYPE
{
    ISOLATED = 4,
    START = 1,
    H_MATRIX = 2,
    V_MATRIX = 3
};

enum FLAG_TYPE
{
    UNVISITED = 0,
    VISITED = 1,
    OVERLAPPED = 2
};



class Codec
{
public:
    Codec();
    Codec(Mat& pic);
    ~Codec();
    Mat encode();
    
    Mat decode(Mat& R);

public:
    Mat pic;
    Mat R;
    Mat flag;
    std::unordered_map<std::string, Point> hashmap;
    std::vector<Rect> overlapRect;
    Point lt;
    Point rb;
    Rect rec;
    int blockCount;
private:
    /*
    private methods
    for encode;
    */
    bool findInitPoint(Point& cur);
    void expandX();
    void expandY();

private:
    /*about overlap*/

    vector<Rect> findOverlapRects();
    void handleNoOverlap();

    void handleOverlapRects(vector<Rect> overlapRects);
    Rect findRectV(int x, int y);
    bool checkLaw2(Rect RectV);
    void divideRectV(Rect RectV);
    bool isCrossOverlap(Rect RectH, Rect RectV);
private:
    /*util methods*/
    std::string point2str(const Point& p);
    bool isWhite(Point p);
    void setFlag(Rect rec, int value);
    void setR(Rect rec, int value);
    bool isOutOfRange(const Point& p);

private:
    bool isIsolated(const Point& lt, const Point& rb);
    void handleIsolated();

private:
    int cols;
    int rows;
};



/**/
// 打印矩形
void showRec(string name, Mat& m, Point lt, Point rb);


// 在R矩阵中画一个矩形
void drawR(Mat& R, Point lt, Point rb, int value);
cv::Mat encoder(cv::Mat& pic);
