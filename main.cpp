#include"encoder.h"
#include"decoder.h"
#include<map>

using namespace std;
using namespace cv;

int main()
{
    //Mat pic = Mat::zeros(8, 8, CV_8UC1);
    //Mat R = Mat::zeros(8, 8, CV_8UC1);

    //setRectInMat(pic, Point(4, 0), Point(7, 3), WHITE);
    //setRectInMat(pic, Point(1, 4), Point(2, 7), WHITE);
    //setRectInMat(pic, Point(0, 5), Point(3, 6), WHITE);
    Mat pic = imread("D:\\newPepper256.bmp", 0);
    imshow("pic",pic);
    waitKey(0);
    Mat R = encoder(pic);
    cout << R << endl;
    pic = decoder(R);
    imshow("re:pic", pic);
    waitKey(0);
    system("pause");
}