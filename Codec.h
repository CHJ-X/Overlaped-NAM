#pragma once
#include<opencv2/core.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

//#define DEBUG
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


// #define WHITE 255

using cv::Mat;
using cv::Point;
using cv::Rect;
using std::string;
using std::vector;


enum NODE_TYPE:uchar
{
    ISOLATED = 4,
    START = 1,
    H_MATRIX = 2,
    V_MATRIX = 3
};

enum FLAG_TYPE :uchar
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


public:
    /*
    * public methods
    * 
    * 
    */
    Mat encode();
    Mat decode(Mat& R);

public:
    Mat pic;
    Mat R;
    int blockCount;

    /* 
    * Rect area include top left Point --> Rect::tl(),
    * but not include bottom right Point --> Rect::br()
    */
    std::vector<Rect> overlapArea;

private:
    Mat flag;
    std::unordered_map<std::string, Point> hashmap;
    std::vector<Rect> overlapRect;
    Point lt;
    Point rb;
    Rect rec;
    int cols;
    int rows;

    const static uchar WHITE = 255;
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
    
    void saveOverlapArea(Rect RectH, Rect RectV);
private:
    /*util methods*/
    std::string point2str(const Point& p);
    bool isWhite(Point p);
    void setFlag(Rect rec, FLAG_TYPE flagValue);
    void setR(Rect rec, NODE_TYPE nodeValue);
    bool isOutOfRange(const Point& p);

    int count();

private:
    bool isIsolated(const Point& lt, const Point& rb);
    void handleIsolated();

private:
    /*
    * private methods
    * for decode;
    */
    bool checkClear(Mat& R, Point lt, Point rb);
    void drawPic(Mat& pic, Point lt, Point rb);
    bool findNextFlag(Mat& R, Point& cur);
    Point findMatchPoint(Mat& R, Point& cur);
};
